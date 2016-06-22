<?php
/**
 * @file
 * Function: cron_export()
 */

namespace QTranslate;
use \qtr;

/**
 * The callback function called from cron_queue 'export'.
 */
function cron_export($export_params) {

  // Make sure that exports do not run in parallel,
  // so that the server is not loaded.
  if (!lock_acquire('export', 3000)) {
    // If we cannot get the lock, just stop the execution, do not return,
    // because after the callback function returns, the cron_queue will
    // remove the item from the queue, no matter whether it is processed or not.
    exit();
  }

  // Allow the export script to run until completion.
  set_time_limit(0);

  // Get the parameters of export.
  $lng = $export_params->lng;
  $export_mode = $export_params->export_mode;
  $preferred_users = $export_params->preferred_users;
  $account = user_load($export_params->uid);

  // Get the full path of the export files.
  $export_dir = variable_get('qtr_export_path', '/var/www/exports');
  $username = strtr(strtolower($account->name), ' ', '_');
  $filename = "$username.$lng";
  $file_tgz = "$export_dir/$filename.tgz";

  // Get the latest translations and diffs with the last snapshot.
  exec("rm -f $file_tgz");
  /*
  qtr::project_diff($origin, $project, $lng,
    $file_diff, $file_ediff, $file_tgz,
    $export_mode, $preferred_users, $account->uid);
  */
  $output = qtr::messages_cat(qtr::messages());

  // Notify the user that the export is done.
  $exports_url = url('exports', array('absolute' => TRUE));
  $params = array(
    'type' => 'notify-that-export-is-done',
    'uid' => $account->uid,
    'project' => $origin . '/' . $project,
    'username' => $account->name,
    'recipient' => $account->name .' <' . $account->mail . '>',
    'export_url_tgz' => "$exports_url/$filename.tgz",
    'output' => $output,
  );
  qtr::queue('notifications', array($params));

  // This export is done, allow any other exports to run.
  lock_release('export');
}