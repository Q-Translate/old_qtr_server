<?php
/**
 * Description of the command 'qtr-vocabulary-del'.
 */
function _qtr_vocabulary_del_drush_command() {
  return array(
    'description' => "Delete a vocabulary.",
    'arguments' => array(
      'name' => 'The name of the vocabulary.',
      'lng' => 'The language of the vocabulary.',
    ),
    'options' => array(),
    'examples' => array(
      "drush qtrv-del ICT sq" => '',
    ),
    'aliases' => array('qtrv-del'),
  );
}

/**
 * Help text for the command 'qtr-vocabulary-del'.
 */
function _qtr_vocabulary_del_drush_help() {
  return dt("Delete a vocabulary.");
}

/**
 * Callback function for the command 'qtr-vocabulary-del'.
 */
function drush_qtranslator_qtr_vocabulary_del($name, $lng) {
  $origin = 'vocabulary';
  $project = $name . '_' . $lng;
  _qtranslator_drush_check_params($origin, $project, $lng);

  qtr::vocabulary_del($name, $lng);
}
