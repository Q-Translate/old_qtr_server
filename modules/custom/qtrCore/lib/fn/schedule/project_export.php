<?php
/**
 * @file
 * Function: schedule_project_export()
 */

namespace QTranslate;
use \qtr;

/**
 * Schedule a project for export. When the request
 * is completed, the user will be notified by email.
 *
 * @param $origin
 *   The origin of the project.
 *
 * @param $project
 *   The name of the project.
 *
 * @param $lng
 *   Translation to be exported.
 *
 * @param $export_mode
 *   The export mode that should be used. It can be one of:
 *   (original | most_liked | preferred).
 *     - The mode 'original' exports the translations of the
 *       original files that were imported.
 *     - The mode 'most_liked' exports the translations with the
 *       highest number of likes.
 *     - The mode 'preferred' gives precedence to the translations
 *       liked by a user or a list of users, despite the number of
 *       likes.
 *
 * @param preferred_users
 *   Comma separated list of usernames. Used only when export_mode
 *   is 'preferred'.
 */
function schedule_project_export($origin, $project, $lng,
  $export_mode = NULL, $preferred_users = NULL)
{
  // Make sure that the given origin and project do exist.
  if (!qtr::project_exists($origin, $project)) {
    $msg = t("The project '!project' does not exist.",
           ['!project' => "$origin/$project"]);
    qtr::messages($msg, 'error');
    return;
  }

  // Check the export_mode.
  if (empty($params['export_mode'])) {
    $params['export_mode'] = 'most_liked';
  }
  if (!in_array($export_mode, array('most_liked', 'preferred', 'original'))) {
    $msg = t("Unknown export mode '!export_mode'.",
             ['!export_mode' => $export_mode]);
    qtr::messages($msg, 'error');
    return;
  }

  // Get and check the list of preferred users.
  if ($export_mode == 'preferred') {
    list($arr_emails, $error_messages) = qtr::utils_get_emails($preferred_users);
    if (!empty($error_messages)) {
      qtr::mesages($error_messages);
      return;
    }
    if (empty($arr_emails)) {
      $account = user_load($GLOBALS['user']->uid);
      $arr_emails = [$account->init];
    }
  }

  // Schedule the project export.
  $queue_params = [
    'origin' => $origin,
    'project' => $project,
    'lng' => $lng,
    'uid' => $GLOBALS['user']->uid,
    'export_mode' => $export_mode,
    'preferred_users' => $arr_emails,
  ];
  qtr::queue('export_project', [$queue_params]);

  // Schedule a notification to each admin of the project.
  $notify_admin = variable_get('qtr_export_notify_admin', TRUE);
  if ($notify_admin) {
    $queue_params['type'] = 'notify-admin-on-export-request';
    $admins = qtr::project_users('admin', $origin, $project, $lng);
    foreach ($admins as $uid => $user) {
      $queue_params['recipient'] = $user->email;
      $queue_params['username'] = $user->name;
      qtr::queue('notifications', [$queue_params]);
    }
  }

  // Return a notification message.
  $msg = t("Export of '!project' is scheduled. You will be notified by email when it is done.",
         ['!project' => "$origin/$project"]);
  qtr::messages($msg);
}
