<?php
/**
 * @file
 * Function: schedule_export()
 */

namespace QTranslate;
use \qtr;

/**
 * Schedule export of translations. When the request
 * is completed, the user will be notified by email.
 *
 * @param $lng
 *   Translation to be exported.
 *
 * @param $export_mode
 *   The export mode that should be used. It can be one of:
 *   (most_liked | preferred).
 *     - The mode 'most_liked' exports the translations with the
 *       highest number of likes.
 *     - The mode 'preferred' gives precedence to the translations
 *       liked by a user or a group of users, despite the number of
 *       likes.
 *
 * @param preferred_users
 *   Comma separated list of usernames. Used only when export_mode
 *   is 'preferred'.
 */
function schedule_export($lng, $export_mode = NULL, $preferred_users = NULL)
{
  // Check the export_mode.
  if (empty($params['export_mode'])) {
    $params['export_mode'] = 'most_liked';
  }
  if ($export_mode != 'most_liked' and $export_mode != 'preferred') {
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
    'lng' => $lng,
    'uid' => $GLOBALS['user']->uid,
    'export_mode' => $export_mode,
    'preferred_users' => $arr_emails,
  ];
  qtr::queue('export', [$queue_params]);

  // Return a notification message.
  $msg = t("Export of '!project' is scheduled. You will be notified by email when it is done.",
         ['!project' => "$origin/$project"]);
  qtr::messages($msg);
}
