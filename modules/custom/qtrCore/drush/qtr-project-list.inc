<?php
/**
 * Description of the command 'qtr-project-list'.
 */
function _qtr_project_list_drush_command() {
  return array(
    'description' => "List imported projects, filtered by origin/project.",
    'arguments' => array(
    ),
    'options' => array(
      'origin' => 'The pattern for matching the origin.',
      'project' => 'The pattern for matching the project.',
    ),
    'examples' => array(
      "drush qtrp-ls --origin=KDE --project=k%" => 'List KDE projects that start with k.',
      "drush qtrp-ls --project=-" => 'Get a list of all origins.'
    ),
    'aliases' => array('qtrp-ls'),
  );
}

/**
 * Help text for the command 'qtr-project-list'.
 */
function _qtr_project_list_drush_help() {
  return dt("List imported projects, filtered by origin/project.

Options origin and project can contain '%' (for LIKE matches).
If --project='-' then only a list of 'origin' is outputed.
");
}

/**
 * Callback function for the command 'qtr-project-list'.
 */
function drush_qtranslator_qtr_project_list() {
  $origin = drush_get_option('origin');
  $project = drush_get_option('project');

  $project_list = qtr::project_ls($origin, $project);
  foreach($project_list as $project)  {
    // Exclude vocabularies.
    if (preg_match('#^vocabulary/#', $project))  continue;
    drush_print($project);
  }
}
