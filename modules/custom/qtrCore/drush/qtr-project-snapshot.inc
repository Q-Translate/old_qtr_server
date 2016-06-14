<?php
/**
 * Description of the command 'qtr-project-snapshot'.
 */
function _qtr_project_snapshot_drush_command() {
  return array(
    'description' => "Make a snapshot of the given origin/project/lng.",
    'arguments' => array(
      'origin' => 'The origin of the project.',
      'project' => 'The name of the project',
      'lng' => 'The language of translations.',
      'comment' => 'A comment about the snapshot.',
    ),
    'options' => array(
      'user'  => 'The username of the user that is making the snapshot.',
    ),
    'examples' => array(
      'drush qtrp-snapshot KDE kdeedu sq "Comment"' => '',
    ),
    'aliases' => array('qtrp-snapshot'),
  );
}

/**
 * Help text for the command 'qtr-project-snapshot'.
 */
function _qtr_project_snapshot_drush_help() {
  return dt("Make a snapshot of the PO files for the given origin/project/lng.
Also generates the diffs with the previous snapshot and saves them in DB.
");
}

/**
 * Callback function for the command 'qtr-project-snapshot'.
 */
function drush_qtranslator_qtr_project_snapshot($origin, $project, $lng, $comment) {
  // Check parameters $origin, $project, $lng.
  _qtranslator_drush_check_params($origin, $project, $lng);

  // Make the snapshot.
  qtr::project_snapshot($origin, $project, $lng, $comment);
  foreach(qtr::messages() as $msg)  drush_log($msg[0], $msg[1]);
}
