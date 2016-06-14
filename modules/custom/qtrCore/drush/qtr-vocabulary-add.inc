<?php
/**
 * Description of the command 'qtr-vocabulary-add'.
 */
function _qtr_vocabulary_add_drush_command() {
  return array(
    'description' => "Create a new vocabulary.",
    'arguments' => array(
      'name' => 'The name of the vocabulary.',
      'lng' => 'The language of the vocabulary.',
    ),
    'options' => array(
      'user'  => 'The username of the user that is importing the vocabulary.',
    ),
    'examples' => array(
      "drush qtrv-add ICT sq" => '',
    ),
    'aliases' => array('qtrv-add'),
  );
}

/**
 * Help text for the command 'qtr-vocabulary-add'.
 */
function _qtr_vocabulary_add_drush_help() {
  return dt("Create  a new vocabulary.");
}

/**
 * Callback function for the command 'qtr-vocabulary-add'.
 */
function drush_qtranslate_qtr_vocabulary_add($name, $lng) {
  // Check the language.
  _qtranslate_drush_check_params(NULL, NULL, $lng);

  // Add the vocabulary.
  qtr::vocabulary_add($name, $lng);
  foreach(qtr::messages() as $msg)  drush_log($msg[0], $msg[1]);
}
