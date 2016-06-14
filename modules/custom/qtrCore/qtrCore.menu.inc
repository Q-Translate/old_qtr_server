<?php
/**
 * Implements hook_menu().
 */
function qtrCore_menu() {

  $items = array();

  $items['admin/config/qtr'] = array(
    'title' => 'Q-Translate',
    'description' => 'Q-Translate Configuration',
    'weight' => -100,
    'page callback' => 'system_admin_menu_block_page',
    'file' => 'system.admin.inc',
    'file path' => drupal_get_path('module', 'system'),
    'access callback' => TRUE,
    //'position' => 'right',
  );

  $items['admin/config/qtr/core'] = array(
    'title'            => 'BTR Core',
    'description'      => 'Q-Translate Core configuration settings.',
    'page callback'    => 'drupal_get_form',
    'page arguments'   => array('qtrCore_config'),
    'access arguments' => array('qtranslator-admin'),
    'file'             => 'admin/core.inc',
  );

  $items['admin/config/qtr/data'] = array(
    'title'            => 'BTR Data',
    'description'      => 'Q-Translate Backup/Restore.',
    'page callback'    => 'drupal_get_form',
    'page arguments'   => ['qtrCore_backup'],
    'access arguments' => ['qtranslator-admin'],
    'file'            => 'admin/data.inc',
  );

  // Synchronize a project with the upstream one.
  $items['project/sync'] = array(
    'type'            => MENU_CALLBACK,
    'title'           => 'Synchronize Project',
    'page callback'   => 'qtrCore_project_sync',
    'file'            => 'callback/project.inc',
    'access callback' => TRUE,
  );

  // Return a list of the imported projects,
  // filtered by the given origin/project.
  $items['project/list'] = array(
    'type'            => MENU_CALLBACK,
    'title'           => 'Project List',
    'page callback'   => 'qtrCore_project_list',
    'file'            => 'callback/project.inc',
    'access callback' => TRUE,
  );

  // Return a full list of all the imported projects and languages.
  $items['project/list/all'] = array(
    'type'            => MENU_CALLBACK,
    'title'           => 'Project List',
    'page callback'   => 'qtrCore_project_list_all',
    'file'            => 'callback/project.inc',
    'access callback' => TRUE,
  );

  // Return a list of vocabularies in JSON format.
  $items['vocabulary/list'] = array(
    'type'            => MENU_CALLBACK,
    'title'           => 'Vocabulary List',
    'page callback'   => 'qtrCore_vocabulary_list',
    'file'            => 'callback/vocabulary_list.inc',
    'access callback' => TRUE,
  );

  // Return a list of the languages
  // for the given origin/project.
  $items['project/langs'] = array(
    'type'            => MENU_CALLBACK,
    'title'           => 'Project List',
    'page callback'   => 'qtrCore_project_langs',
    'file'            => 'callback/project.inc',
    'access callback' => TRUE,
  );

  // Return an archive of the PO files for a given origin/project/lng.
  $items['project/export'] = array(
    'type'            => MENU_CALLBACK,
    'title'           => 'Project Export',
    'page callback'   => 'qtrCore_project_export',
    'file'            => 'callback/project_export.inc',
    'access callback' => TRUE,
  );

  // Return the items of a vocabulary and all the corresponding translations.
  $items['vocabulary/export'] = array(
    'type'            => MENU_CALLBACK,
    'title'           => 'Export Vocabulary',
    'page callback'   => 'qtrCore_vocabulary_export',
    'file'            => 'callback/vocabulary_export.inc',
    'access callback' => TRUE,
  );

  // Return as plain-text the diff of the PO file for a given origin/project/lng.
  // It gets the parameters origin/project/lng[/nr[/ediff]].
  $items['project/diff'] = array(
    'type'            => MENU_CALLBACK,
    'title'           => 'Project Diffs',
    'page callback'   => 'qtrCore_project_diff',
    'file'            => 'callback/diff.inc',
    'access callback' => TRUE,
  );

  // Return as a download the diff of the PO file for a given origin/project/lng.
  $items['project/diff-dl'] = array(
    'type'            => MENU_CALLBACK,
    'title'           => 'Diff Download',
    'page callback'   => 'qtrCore_project_diff_download',
    'file'            => 'callback/diff.inc',
    'access callback' => TRUE,
  );

  // Return as a download the latest diff of the PO file for a given origin/project/lng.
  $items['project/diff-latest'] = array(
    'type'            => MENU_CALLBACK,
    'title'           => 'Latest Diff Download',
    'page callback'   => 'qtrCore_project_diff_latest',
    'file'            => 'callback/diff_latest.inc',
    'access callback' => TRUE,
  );

  // Make a project snapshot for a given origin/project/lng.
  $items['project/snapshot'] = array(
    'type'            => MENU_CALLBACK,
    'title'           => 'Make Snapshot',
    'page callback'   => 'qtrCore_project_snapshot',
    'file'            => 'callback/snapshot.inc',
    'access callback' => TRUE,
  );

  // The link that is sent to the user for downloading an exported file.
  // The URL is of the form 'exports/filename.tgz'
  // and the filename is passed as a parameter to the function.
  $items['exports'] = array(
    'type'            => MENU_CALLBACK,
    'title'           => 'Download Exported File',
    'page callback'   => 'qtrCore_get_exported_file',
    'file'            => 'callback/exports.inc',
    'access callback' => TRUE,
  );

  // Return a string context, related projects (where a string is found), etc.
  $items['string_details'] = array(
    'type'            => MENU_CALLBACK,
    'title'           => 'String details',
    'page callback'   => 'qtrCore_string_details',
    'file'            => 'callback/string_details.inc',
    'access callback' => TRUE,
  );

  // Autocomplete callback for project textfields.
  $items['auto/project'] = array(
    'type'            => MENU_CALLBACK,
    'title'           => 'Project autocomplete',
    'page callback'   => 'qtrCore_project_autocomplete',
    'file'            => 'callback/ajax.inc',
    'access callback' => TRUE,
  );

  // Autocomplete callback for origin textfields.
  $items['auto/origin'] = array(
    'type'            => MENU_CALLBACK,
    'title'           => 'Origin autocomplete',
    'page callback'   => 'qtrCore_origin_autocomplete',
    'file'            => 'callback/ajax.inc',
    'access callback' => TRUE,
  );

  // Autocomplete callback for user textfields.
  $items['auto/user'] = array(
    'type'            => MENU_CALLBACK,
    'title'           => 'User autocomplete',
    'page callback'   => 'qtrCore_user_autocomplete',
    'file'            => 'callback/ajax.inc',
    'access callback' => TRUE,
  );

  // Autocomplete callback for strings.
  $items['auto/string'] = array(
    'type'            => MENU_CALLBACK,
    'title'           => 'String autocomplete',
    'page callback'   => 'qtrCore_string_autocomplete',
    'file'            => 'callback/ajax.inc',
    'access callback' => TRUE,
  );

  // Return latest translations.
  $items['latest'] = array(
    'type'            => MENU_CALLBACK,
    'title'           => 'Latest Translation Feedback',
    'page callback'   => 'qtrCore_latest',
    'file'            => 'callback/latest.inc',
    'access callback' => TRUE,
  );

  // Return a RSS feed.
  $items['rss-feed'] = array(
    'type'            => MENU_CALLBACK,
    'title'           => 'Latest Translation Feedback',
    'page callback'   => 'qtrCore_rss_feed',
    'file'            => 'callback/rss_feed.inc',
    'access callback' => TRUE,
  );

  // Return a random twitter.
  $items['tweet'] = array(
    'type'            => MENU_CALLBACK,
    'title'           => 'Return a random string (and translations) in a form suitable for twitter.',
    'page callback'   => 'qtrCore_tweet',
    'file'            => 'callback/tweet.inc',
    'access callback' => TRUE,
  );

  // Return a json list of languages and their details.
  $items['languages'] = array(
    'type'            => MENU_CALLBACK,
    'title'           => 'Return a json list of languages and their details.',
    'page callback'   => 'qtrCore_languages',
    'file'            => 'callback/languages.inc',
    'access callback' => TRUE,
  );

  return $items;
}
