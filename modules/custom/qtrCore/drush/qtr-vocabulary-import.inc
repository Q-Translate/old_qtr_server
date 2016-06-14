<?php
/**
 * Description of the command 'qtr-vocabulary-import'.
 */
function _qtr_vocabulary_import_drush_command() {
  return array(
    'description' => "Import the terms and translations of a vocabulary from a text file.",
    'arguments' => array(
      'name' => 'The name of the vocabulary.',
      'lng' => 'The language of the vocabulary.',
      'file' => 'Text file with terms and translations (having the same format as export TXT1).',
    ),
    'options' => array(
      'user'  => 'The username of the user that is importing the vocabulary.',
    ),
    'examples' => array(
      "drush qtrv-import ICT sq $(pwd)/ICT_sq.txt" => '',
    ),
    'aliases' => array('qtrv-import'),
  );
}

/**
 * Help text for the command 'qtr-vocabulary-import'.
 */
function _qtr_vocabulary_import_drush_help() {
  return dt("Import the terms and translations of a vocabulary from a text file.");
}

/**
 * Callback function for the command 'qtr-vocabulary-import'.
 */
function drush_qtranslate_qtr_vocabulary_import($name, $lng, $file) {
  // Check the language.
  _qtranslate_drush_check_params(NULL, NULL, $lng);

  // Import the vocabulary.
  qtr::vocabulary_import($name, $lng, $file);
  foreach (qtr::messages() as $msg)  drush_log($msg[0], $msg[1]);
}
