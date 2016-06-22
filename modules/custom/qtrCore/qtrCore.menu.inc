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
    'title'            => 'QTR Core',
    'description'      => 'Q-Translate Core configuration settings.',
    'page callback'    => 'drupal_get_form',
    'page arguments'   => array('qtrCore_config'),
    'access arguments' => array('qtranslate-admin'),
    'file'             => 'admin/core.inc',
  );

  $items['admin/config/qtr/data'] = array(
    'title'            => 'QTR Data',
    'description'      => 'Q-Translate Backup/Restore.',
    'page callback'    => 'drupal_get_form',
    'page arguments'   => ['qtrCore_backup'],
    'access arguments' => ['qtranslate-admin'],
    'file'            => 'admin/data.inc',
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

  // Autocomplete callback for user textfields.
  $items['auto/user'] = array(
    'type'            => MENU_CALLBACK,
    'title'           => 'User autocomplete',
    'page callback'   => 'qtrCore_user_autocomplete',
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