<?php
/**
 * @file
 * Function: cron_import_project()
 */

namespace QTranslate;
use \qtr;

/**
 * The callback function called from cron_queue 'import_project'.
 */
function cron_import_project($params) {

  // Make sure that imports do not run in parallel,
  // so that the server is not overloaded.
  if (!lock_acquire('import_project', 3000)) {
    // If we cannot get the lock, just stop the execution, do not return,
    // because after the callback function returns, the cron_queue will
    // remove the item from the queue, no matter whether it is processed or not.
    exit();
  }

  // Allow the import script to run until completion.
  set_time_limit(0);

  // Get the parameters.
  $account = user_load($params->uid);
  $file = file_load($params->fid);
  $origin = $params->origin;
  $project = $params->project;

  // Check that the file exists.
  if (!file_exists($file->uri)) {
    lock_release('import_project');
    return;
  }

  // Create a temporary directory.
  $tmpdir = '/tmp/' . sha1_file($file->uri);
  mkdir($tmpdir, 0700);

  // Copy the file there and extract it.
  file_unmanaged_copy($file->uri, $tmpdir);
  exec("cd $tmpdir ; dtrx -q -n -f $file->filename 2>/dev/null");

  // Create the project.
  qtr::project_add($origin, $project, "$tmpdir/pot/", $account->uid);
  $output = qtr::messages_cat(qtr::messages());

  // Import the PO files for each language.
  $langs = qtr::languages_get();
  $dir_list = glob("$tmpdir/*", GLOB_ONLYDIR);
  foreach ($dir_list as $dir) {
    $lng = basename($dir);
    if ($lng == 'pot')  continue;
    if (!in_array($lng, $langs))  continue;

    // Import PO file and translations.
    qtr::project_import($origin, $project, $lng, "$tmpdir/$lng/", $account->uid);
    $output .= qtr::messages_cat(qtr::messages());
  }


  // Get the url of the client site.
  module_load_include('inc', 'qtrCore', 'includes/sites');
  $client_url = qtr::utils_get_client_url($lng);

  // Notify the user that the project import is done.
  $params = array(
    'type' => 'notify-that-project-import-is-done',
    'uid' => $account->uid,
    'username' => $account->name,
    'recipient' => $account->name .' <' . $account->mail . '>',
    'project' => $origin . '/' . $project,
    'url' => "$client_url/qtr/project/$origin/$project/$lng/dashboard",
    'output' => $output,
  );
  qtr::queue('notifications', array($params));

  // Cleanup, remove the temp dir and delete the file.
  exec("rm -rf $tmpdir/");
  file_delete($file, TRUE);

  // This import is done, allow any other imports to run.
  lock_release('import_project');
}
