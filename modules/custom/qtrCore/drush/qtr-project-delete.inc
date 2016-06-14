<?php
/**
 * Description of the command 'qtr-project-delete'.
 */
function _qtr_project_delete_drush_command() {
  return array(
    'description' => "Delete a project from the DB.",
    'arguments' => array(
    ),
    'options' => array(
      'origin' => 'The origin of the project to be deleted.',
      'project' => 'The name of the project to be deleted.',
      'erase' => 'If true, then snapshots and diffs are deleted as well.',
      'purge' => 'If true, then dangling strings are deleted as well.',
    ),
    'examples' => array(
      "drush qtrp-del --origin=KDE --project=kturtle" => 'Delete KDE/kturtle from the DB.',
    ),
    'aliases' => array('qtrp-del'),
  );
}

/**
 * Help text for the command 'qtr-project-delete'.
 */
function _qtr_project_delete_drush_help() {
  return "Delete everything related to the given origin and project.

If no project is given, then all the projects of the given origin
will be deleted. If the origin is NULL, then all the projects
of the given name (from any origin) will be deleted.";
}

/**
 * Callback function for the command 'qtr-project-delete'.
 */
function drush_qtranslate_qtr_project_delete() {
  $origin = drush_get_option('origin');
  $project = drush_get_option('project');
  _qtranslate_drush_check_params($origin, $project);

  $erase = drush_get_option('erase');
  $erase = ($erase == 'false' ? FALSE : TRUE);

  $purge = drush_get_option('purge');
  $purge = ($purge == 'false' ? FALSE : TRUE);

  qtr::project_del($origin, $project, $erase, $purge);
}
