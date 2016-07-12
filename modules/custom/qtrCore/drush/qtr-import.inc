<?php
/**
 * Description of the command 'qtr-import'.
 */
function _qtr_import_drush_command() {
  return array(
    'description' => "Import translation from a text file.",
    'arguments' => array(
      'lng'     => 'The language of translation (de, fr, sq, en_GB, etc.)',
      'file' => 'A text file that has the translation of each verse on a separate line.',
    ),
    'options' => array(
      'user'  => 'The username of the user that has made the translations.',
    ),
    'examples' => array(
      "drush qtr-imp --user=pickthall en \$(pwd)/pickthall.txt" => 'Import the Pickthall English translations.',
    ),
    'aliases' => array('qtr-imp'),
  );
}

/**
 * Help text for the command 'qtr-import'.
 */
function _qtr_import_drush_help() {
  return dt('Import translation from a text file.

The option --user is required because it declares the author of translations.
The translation file is a text file that has the translation of each verse on a
separate line.
');
}

/**
 * Callback function for the command 'qtr-import'.
 */
function drush_qtranslate_qtr_import($lng, $file) {
  global $user;
  if (!$user->uid) {
    drush_log(t('You must specify a user with the option --user.'), 'error');
    drupal_exit();
  }

  // Check user permissions.
  _drush_qtranslate_qtr_import_check($lng);

  // Check that there is such a file.
  if (!file_exists($file)) {
    drush_log(t('File "!file" does not exist.', ['!file' => $file]), 'error');
    drupal_exit();
  }

  // Import translations.
  qtr::import_translations($lng, $file, $user->uid);
}

/**
 * Check user permissions.
 */
function _drush_qtranslate_qtr_import_check($lng) {
  global $user;

  // Check user access permissions.
  if (!user_access('qtranslate-translate')) {
    drush_log(t('User "!user" has no rights for suggesting translations!', ['!user' => $user->name]), 'error');
    drupal_exit();
  }
  if (!user_access('qtranslate-like')) {
    drush_log(t('User "!user" has no rights for submiting likes!', ['!user' => $user->name]), 'error');
    drupal_exit();
  }

  // Get and check the parameter 'lng'.
  if (!qtr::lng_check($lng)) {
    drush_log(t('Language "!lng" is not supported.', array('!lng' => $lng)), 'error');
    drupal_exit();
  }

  // Make sure that this user can make translations for the given language.
  if ($lng != $user->translation_lng) {
    drush_log(t('User "!user" cannot submit translations for the language "!lng"',
        ['!user' => $user->name, '!lng' => $lng]
      ), 'error');
    drupal_exit();
  }
}
