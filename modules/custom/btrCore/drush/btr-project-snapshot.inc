<?php
/**
 * Description of the command 'btr-project-snapshot'.
 */
function _btr_project_snapshot_drush_command() {
  return array(
    'description' => "Make a snapshot of the given origin/project/lng.",
    'arguments' => array(
      'origin' => 'The origin of the project.',
      'project' => 'The name of the project',
      'lng' => 'The language of translations.',
      'comment' => 'A comment about the snapshot.',
    ),
    'options' => array(
    ),
    'examples' => array(
      'drush btrp-snapshot KDE kdeedu sq "Comment"' => '',
    ),
    'aliases' => array('btrp-snapshot'),
  );
}

/**
 * Help text for the command 'btr-project-snapshot'.
 */
function _btr_project_snapshot_drush_help() {
  return dt("Make a snapshot of the PO files for the given origin/project/lng.
Also generates the diffs with the previous snapshot and saves them in DB.
");
}

/**
 * Callback function for the command 'btr-project-snapshot'.
 */
function drush_btranslator_btr_project_snapshot($origin, $project, $lng, $comment) {
  btr::project_snapshot($origin, $project, $lng, $comment);
}