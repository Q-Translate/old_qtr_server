<?php
/**
 * @file
 * Drush commands for importing/exporting *.po files.
 */

require_once(dirname(__FILE__) . '/qtr-project-list.inc');
require_once(dirname(__FILE__) . '/qtr-project-add.inc');
require_once(dirname(__FILE__) . '/qtr-project-import.inc');
require_once(dirname(__FILE__) . '/qtr-project-delete.inc');
require_once(dirname(__FILE__) . '/qtr-project-export.inc');
require_once(dirname(__FILE__) . '/qtr-project-diff.inc');
require_once(dirname(__FILE__) . '/qtr-project-diff-ls.inc');
require_once(dirname(__FILE__) . '/qtr-project-diff-get.inc');
require_once(dirname(__FILE__) . '/qtr-project-snapshot.inc');
require_once(dirname(__FILE__) . '/qtr-vote-import.inc');
require_once(dirname(__FILE__) . '/qtr-vocabulary-add.inc');
require_once(dirname(__FILE__) . '/qtr-vocabulary-del.inc');
require_once(dirname(__FILE__) . '/qtr-vocabulary-list.inc');
require_once(dirname(__FILE__) . '/qtr-vocabulary-get.inc');
require_once(dirname(__FILE__) . '/qtr-vocabulary-import.inc');

/**
 * Implementation of hook_drush_command().
 *
 * @return
 *   An associative array describing commands.
 */
function qtranslator_drush_command() {
  $items = array(
    'qtr-project-list' => _qtr_project_list_drush_command(),
    'qtr-project-add' => _qtr_project_add_drush_command(),
    'qtr-project-import' => _qtr_project_import_drush_command(),
    'qtr-project-delete' => _qtr_project_delete_drush_command(),
    'qtr-project-export' => _qtr_project_export_drush_command(),
    'qtr-project-diff' => _qtr_project_diff_drush_command(),
    'qtr-project-diff-ls' => _qtr_project_diff_ls_drush_command(),
    'qtr-project-diff-get' => _qtr_project_diff_get_drush_command(),
    'qtr-project-snapshot' => _qtr_project_snapshot_drush_command(),
    'qtr-vote-import' => _qtr_vote_import_drush_command(),
    'qtr-vocabulary-add' => _qtr_vocabulary_add_drush_command(),
    'qtr-vocabulary-del' => _qtr_vocabulary_del_drush_command(),
    'qtr-vocabulary-list' => _qtr_vocabulary_list_drush_command(),
    'qtr-vocabulary-get' => _qtr_vocabulary_get_drush_command(),
    'qtr-vocabulary-import' => _qtr_vocabulary_import_drush_command(),
  );

  // These will appear in the list of topics when `drush topic` is executed.
  // To view this topic, run `drush --include=/full/path/to/examples topic`
  $items['qtranslator-exposition'] = array(
    'description' => dt('Importing and exporting PO files from Q-Translate.'),
    'hidden' => TRUE,
    'topic' => TRUE,
    'callback' => 'drush_print_file',
    'callback arguments' => array(dirname(__FILE__) . '/qtranslator-topic.txt'),
  );

  return $items;
}

/**
 * Implementation of hook_drush_help().
 *
 * @param
 *   A string with the help section (prepend with 'drush:')
 *
 * @return
 *   A string with the help text for your command.
 */
function qtranslator_drush_help($section) {
  switch ($section) {
    case 'drush:qtr-project-list':
      return _qtr_project_list_drush_help();
    case 'drush:qtr-project-add':
      return _qtr_project_add_drush_help();
    case 'drush:qtr-project-import':
      return _qtr_project_import_drush_help();
    case 'drush:qtr-project-export':
      return _qtr_project_export_drush_help();
    case 'drush:qtr-project-delete':
      return _qtr_project_delete_drush_help();
    case 'drush:qtr-project-diff':
      return _qtr_project_diff_drush_help();
    case 'drush:qtr-project-diff-ls':
      return _qtr_project_diff_ls_drush_help();
    case 'drush:qtr-project-diff-get':
      return _qtr_project_diff_get_drush_help();
    case 'drush:qtr-project-snapshot':
      return _qtr_project_snapshot_drush_help();
    case 'drush:qtr-vote-import':
      return _qtr_vote_import_drush_help();
    case 'drush:qtr-vocabulary-add':
      return _qtr_vocabulary_add_drush_help();
    case 'drush:qtr-vocabulary-del':
      return _qtr_vocabulary_del_drush_help();
    case 'drush:qtr-vocabulary-list':
      return _qtr_vocabulary_list_drush_help();
    case 'drush:qtr-vocabulary-get':
      return _qtr_vocabulary_get_drush_help();
    case 'drush:qtr-vocabulary-import':
      return _qtr_vocabulary_import_drush_help();

    case 'meta:qtranslator:title':
      return dt("Q-Translate Commands");
    case 'meta:qtranslator:summary':
      return dt("Handle the import/export of PO/POT files for Q-Translate.");
  }
}

/**
 * Check that the parameters $origin, $project and $lng exist (are valid).
 */
function _qtranslator_drush_check_params($origin, $project = NULL, $lng = NULL) {
  // Make sure that the given origin and project do exist.
  if ($project !== NULL and !qtr::project_exists($origin, $project)) {
    drush_log(t("The project '!project' does not exist.",
        array('!project' => "$origin/$project")), 'error');
    drupal_exit();
  }

  // Make sure that the given language exists.
  if ($lng !== NULL and !in_array($lng, qtr::languages_get())) {
    drush_log(t("The language '!lng' does not exist.", ['!lng' => "$lng"]), 'error');
    drupal_exit();
  }
}
