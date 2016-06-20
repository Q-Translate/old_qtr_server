<?php
/**
 * @file
 * Callback function for menu path project/export
 */


/**
 * Callback function for the menu 'project/export'
 *
 * Return an archive of PO files for a given origin/project/lng.
 *
 * Does not allow concurrent exports because they may affect the performance
 * of the server.  Parameters are received from the request (either GET or
 * POST). They are:
 *
 *  - origin
 *      Origin of the project.
 *
 *  - project
 *      The name of the project.
 *
 *  - lng
 *      Translation to be exported.
 *
 *  - export_mode
 *      Mode of export: most_liked | preferred | original
 *
 *      Default is 'most_liked' which exports the most liked
 *      translations and suggestions.
 *
 *      The export mode 'original' exports the translations of the
 *      original file that was imported (useful for making diffs).
 *
 *      The export mode 'preferred' gives priority to translations
 *      that are liked by a certain user or a group of users. It
 *      requires an additional argument (preferred_users) to specify
 *      the user (or a list of users) whose translations are
 *      preferred.
 *
 * - preferred_users
 *      Comma separated list of usernames of the preferred users.
 *
 * Examples:
 *   curl -k 'https://dev.qtranslate.org/project/export?origin=test&project=kturtle&lng=sq' > test1.tgz
 *   curl -k https://dev.qtranslate.org/project/export \
 *        -d 'origin=test&project=kturtle&lng=sq&export_mode=original' > test1.tgz
 *   curl -k https://dev.qtranslate.org/project/export \
 *        -d origin=test -d project=kturtle -d lng=sq \
 *        -d export_mode=preferred \
 *        -d preferred_users=user1,user2 > test1.tgz
 */
function qtrCore_project_export() {
  // Try to avoid concurrent exports because they
  // may affect the performance of the server.
  if (!lock_acquire('export_project')) {
    print t("Error: Server is currently busy. Please try to export again later.");
    drupal_exit();
  }

  // Get the parameters.
  $origin = check_plain($_REQUEST['origin']);
  $project = check_plain($_REQUEST['project']);
  $lng = check_plain($_REQUEST['lng']);
  $export_mode = check_plain($_REQUEST['export_mode']);
  $preferred_users = check_plain($_REQUEST['preferred_users']);

  // Only a project admin can request a direct export.
  if (!qtr::user_is_project_admin($origin, $project, $lng)) {
    print t("You don't have enough rights for this export.");
    drupal_exit();
  }

  // Make sure that the given origin and project do exist.
  if (!qtr::project_exists($origin, $project)) {
    print t("The project '!project' does not exist.",
      array('!project' => "$origin/$project"));
    drupal_exit();
  }

  // Check the export_mode.
  if (empty($export_mode))  $export_mode = 'most_liked';
  if (!in_array($export_mode, array('most_liked', 'preferred', 'original'))) {
    print t("Unknown export mode '!export_mode'.",
      array('!export_mode' => $export_mode));
    drupal_exit();
  }

  // Get and check the list of preferred users.
  if ($export_mode == 'preferred') {
    if (empty($preferred_users)) {
      $preferred_users = $GLOBALS['user']->name;
    }
    list($arr_emails, $error_messages) = qtr::utils_get_emails($preferred_users);
    if (!empty($error_messages)) {
      foreach ($error_messages as $msg) { print $msg[0] . "\n"; }
      drupal_exit();
    }
  }

  // Export translations of the project.
  set_time_limit(0);
  $export_dir = exec('mktemp -d');
  qtr::project_export($origin, $project, $lng, $export_dir, $export_mode, $arr_emails);
  $file_tgz = tempnam('/tmp', 'export_');
  exec("tar -cz -f $file_tgz -C $export_dir .");

  // Send the headers.
  $filename = "$origin-$project-$lng.tgz";
  header('Content-Type: application/x-compressed');
  header('Content-Disposition: attachment; filename="' . $filename . '"');
  header('Content-Transfer-Encoding: binary');
  header("Content-Length: " . filesize($file_tgz));

  // Open the file in a binary mode and dump it to the browser.
  $fp = fopen($file_tgz, 'rb');
  fpassthru($fp);

  // Cleanup temporary files.
  unlink($file_tgz);
  exec("rm -rf $export_dir");

  // Stop execution.
  lock_release('export_project');
  drupal_exit();
}
