<?php
/**
 * Description of the command 'qtr-project-export'.
 */
function _qtr_project_export_drush_command() {
  return array(
    'description' => "Export the PO files of a project from the DB.",
    'arguments' => array(
      'origin' => 'The origin of the project (ubuntu, GNOME, KDE, etc.)',
      'project' => 'The name of the project that is being exported.',
      'lng' => 'The language of translation (de, fr, sq, en_GB, etc.)',
      'path' => 'The directory where the PO files will be exported.',
    ),
    'options' => array(
      'export-mode' => "'most_liked' (default) or 'preferred' or 'original'",
      'preferred-users' => 'Comma separated list of the preferred users.',
      'user' => 'The username of the user who is requesting the export.',
    ),
    'examples' => array(
      "drush qtrp-export KDE kdeedu sq $(pwd)/kdeedu/" => 'Export KDE/kdeedu/sq into kdeedu/.',
    ),
    'aliases' => array('qtrp-export'),
  );
}

/**
 * Help text for the command 'qtr-project-export'.
 */
function _qtr_project_export_drush_help() {
  return "Export the PO files of a project from the DB.

The export mode 'most_liked' (which is the default one) exports the
most liked translations and suggestions.

The export mode 'preferred' gives priority to translations that are
liked by a certain user or a group of users. It requires an additional
option (preferred_users) to specify the user (or a list of users)
whose translations are preferred. If a string has no translation that
is liked by any of the preferred users, then the most liked
translation is exported.

The export mode 'original' exports the translations of the original
file that was imported (useful for making an initial snapshot of the
project).
";
}

/**
 * Callback function for the command 'qtr-project-export'.
 */
function drush_qtranslate_qtr_project_export($origin, $project, $lng, $path) {
  // Check parameters $origin, $project, $lng.
  _qtranslate_drush_check_params($origin, $project, $lng);

  // Check the export_mode.
  $export_mode = drush_get_option('export-mode', 'most_liked');
  if (!in_array($export_mode, ['most_liked', 'preferred', 'original'])) {
    drush_log(t("Unknown export mode '!export_mode'.",
        ['!export_mode' => $export_mode]), 'error');
    drupal_exit();
  }

  // Get the preferred users.
  if ($export_mode == 'preferred') {
    $preferred_users = drush_get_option('preferred-users', NULL);
    list($arr_emails, $error_messages) = qtr::utils_get_emails($preferred_users);
    if (!empty($error_messages)) {
      foreach ($error_messages as $msg)  drush_log($msg[0], $msg[1]);
      drupal_exit();
    }
  }

  // Export the project.
  qtr::project_export($origin, $project, $lng, $path, $export_mode, $arr_emails);
  foreach (qtr::messages() as $msg)  drush_log($msg[0], $msg[1]);
}
