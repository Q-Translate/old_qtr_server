<?php
/**
 * Description of the command 'qtr-project-diff-ls'.
 */
function _qtr_project_diff_ls_drush_command() {
  return array(
    'description' => "Show a list of diffs for the given origin/project/lng.",
    'arguments' => array(
      'origin' => 'The origin of the project.',
      'project' => 'The name of the project',
      'lng' => 'The language of translations.',
    ),
    'options' => array(
    ),
    'examples' => array(
      'drush qtrp-diff-ls KDE kdeedu sq' => '',
    ),
    'aliases' => array('qtrp-diff-ls'),
  );
}

/**
 * Help text for the command 'qtr-project-diff-ls'.
 */
function _qtr_project_diff_ls_drush_help() {
  return dt("Show a list of diffs for the given origin/project/lng.");
}

/**
 * Callback function for the command 'qtr-project-diff-ls'.
 */
function drush_qtranslator_qtr_project_diff_ls($origin, $project, $lng) {
  // Check parameters $origin, $project, $lng.
  _qtranslator_drush_check_params($origin, $project, $lng);

  $arr_diffs = array();
  $diff_list = qtr::project_diff_list($origin, $project, $lng);
  foreach ($diff_list as $diff) {
    // Separate fields by TAB.
    $arr_diffs[] = implode("\t", array($diff->nr, $diff->time, $diff->comment));
  }
  $output = implode("\n", $arr_diffs) . "\n";
  print $output;
}
