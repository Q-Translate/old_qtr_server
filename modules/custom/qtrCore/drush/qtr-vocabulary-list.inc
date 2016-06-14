<?php
/**
 * Description of the command 'qtr-vocabulary-list'.
 */
function _qtr_vocabulary_list_drush_command() {
  return array(
    'description' => "List vocabularies.",
    'arguments' => array(),
    'options' => array(
      'lng' => 'The code of the language.',
      'name' => 'The pattern for matching the vocabulary name.',
    ),
    'examples' => array(
      "drush qtrv-ls --lng=sq" => 'List vocabularies of language sq.',
      "drush qtrv-ls --name=ICT%" => 'List vocabularies that start with ICT.',
    ),
    'aliases' => array('qtrv-ls'),
  );
}

/**
 * Help text for the command 'qtr-vocabulary-list'.
 */
function _qtr_vocabulary_list_drush_help() {
  return dt("List vocabularies.

The option 'lng' is the code of the language, for example 'sq', 'fr', 'de', etc.
The option 'name' can contain '%' (for LIKE matches).
");
}

/**
 * Callback function for the command 'qtr-vocabulary-list'.
 */
function drush_qtranslate_qtr_vocabulary_list() {
  $name = drush_get_option('name', '%');
  $lng = drush_get_option('lng', '%');

  $vocabulary_list = qtr::vocabulary_ls($lng, $name);

  foreach ($vocabulary_list as $lng => $vocabularies) {
    foreach ($vocabularies as $vocabulary) {
      drush_print("$lng\t$vocabulary");
    }
  }
}
