<?php
/**
 * Description of the command 'qtr-project-diff-get'.
 */
function _qtr_project_diff_get_drush_command() {
  return array(
    'description' => "Get the content of the specified diff.",
    'arguments' => array(
      'origin' => 'The origin of the project.',
      'project' => 'The name of the project',
      'lng' => 'The language of translations.',
      'nr' => 'The number of diff to be returned (as returned by qtr-project-diff-ls).',
    ),
    'options' => array(
      'format' => 'The diff format: \'diff\' or \'ediff\' (default \'diff\').',
      'output' => 'The file where to save the output.'
    ),
    'examples' => array(
      'drush qtrp-diff-get KDE kdeedu sq 1 > test.diff' => '',
      'drush qtrp-diff-get KDE kdeedu sq 2 --output=/tmp/test.ediff --format=ediff' => '',
    ),
    'aliases' => array('qtrp-diff-get'),
  );
}

/**
 * Help text for the command 'qtr-project-diff-get'.
 */
function _qtr_project_diff_get_drush_help() {
  return dt("Get the content of the specified diff.");
}

/**
 * Callback function for the command 'qtr-project-diff-get'.
 */
function drush_qtranslate_qtr_project_diff_get($origin, $project, $lng, $nr) {
  // Check parameters $origin, $project, $lng.
  _qtranslate_drush_check_params($origin, $project, $lng);

  // Get the options.
  $format = drush_get_option('format');
  $format = ($format == 'ediff' ? 'ediff' : 'diff');
  $file = drush_get_option('output', NULL);

  // Get the diff.
  $diff = qtr::project_diff_get($origin, $project, $lng, $nr, $format);

  // Output it.
  if ($file === NULL) {
    print $diff;
  }
  else {
    file_put_contents($file, $diff);
  }
}
