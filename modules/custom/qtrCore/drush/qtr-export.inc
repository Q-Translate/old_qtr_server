<?php
/**
 * Description of the command 'qtr-export'.
 */
function _qtr_export_drush_command() {
  return array(
    'description' => "Export the translations of a language.",
    'arguments' => array(
      'lng' => 'The language of translation (de, fr, sq, en_GB, etc.)',
      'path' => 'The directory where the translation file will be exported.',
    ),
    'options' => array(
      'export-mode' => "'most_liked' (default) or 'preferred'",
      'preferred-users' => 'Comma separated list of the preferred users.',
      'user' => 'The username of the user who is requesting the export.',
    ),
    'examples' => array(
      "drush qtr-export sq $(pwd)" => 'Export the most likes Albanian translations into the current directory.',
    ),
    'aliases' => array('qtr-exp'),
  );
}

/**
 * Help text for the command 'qtr-export'.
 */
function _qtr_export_drush_help() {
  return "Export the translations of a language..

The export mode 'most_liked' (which is the default one) exports the
most liked translations.

The export mode 'preferred' gives priority to translations that are
liked by a certain user or a group of users. It requires an additional
option (preferred_users) to specify the user (or a list of users)
whose translations are preferred. If a string has no translation that
is liked by any of the preferred users, then the most liked
translation is exported.
";
}

/**
 * Callback function for the command 'qtr-export'.
 */
function drush_qtranslate_qtr_export($origin, $project, $lng, $path) {
  // Check parameters $origin, $project, $lng.
  _qtranslate_drush_check_params($origin, $project, $lng);

  // Check the export_mode.
  $export_mode = drush_get_option('export-mode', 'most_liked');
  if ($export_mode != 'most_liked' and $export_mode != 'preferred') {
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
  qtr::export($lng, $path, $export_mode, $arr_emails);
  //foreach (qtr::messages() as $msg)  drush_log($msg[0], $msg[1]);
}
