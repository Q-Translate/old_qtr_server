<?php
/**
 * @file
 * Importing translations from PO files.
 */

namespace BTranslator;
use \btr;

/**
 * Importing translations from PO files.
 *
 * It is like a bulk translation and voting service. For any
 * translation in the PO files, it will be added as a suggestion if
 * such a translation does not exist, or it will just be voted if such
 * a translation already exists. In case that the translation already
 * exists but its author is not known, then you (the user who makes
 * the import) will be recorded as the author of the translation.
 *
 * This can be useful for translators if they prefer to work off-line
 * with PO files. They can export the PO files of a project, work on
 * them with desktop tools (like Lokalize) to translate or correct
 * exported translations, and then import back to B-Translator the
 * translated/corrected PO files.
 *
 * @param $uid
 *   ID of the user that has uploaded the file.
 *
 * @param $lng
 *   Language of translations.
 *
 * @param $path
 *   A directory with PO files to be used for import.
 *
 * @return
 *   Array of notification messages; each notification message
 *   is an array of a message and a type, where type can be one of
 *   'status', 'warning', 'error'.
 */
function translations_import($uid, $lng, $path) {
  // Switch to the user that has uploaded the file.
  global $user;
  $original_user = $user;
  $old_state = drupal_save_session();
  drupal_save_session(FALSE);
  $user = user_load($uid);

  // Get the mail of the user.
  $umail = $user->init;

  // Get a list of all PO files on the path.
  $files = file_scan_directory($path, '/.*\.po$/');

  // Import the PO files.
  module_load_include('php', 'btrCore', 'lib/gettext/POParser');
  foreach ($files as $file) {
    // Parse the PO file.
    $parser = new POParser;
    $entries = $parser->parse($file->uri);

    // Process each gettext entry.
    foreach ($entries as $entry) {
      // Get the string sguid.
      $sguid = _get_sguid($entry, $uid);
      if ($sguid === NULL)  continue;

      // Get the translation.
      $translation = is_array($entry['msgstr']) ? implode("\0", $entry['msgstr']) : $entry['msgstr'];
      if (trim($translation) === '')  continue;

      // Add the translation for this string.
      _add_translation($sguid, $lng, $translation, $umail);
    }
  }

  // Switch back to the original user.
  $user = $original_user;
  drupal_save_session($old_state);
}

/**
 * Returns the sguid of the string.
 *
 * If such a string does not exist, insert it into the DB.  However,
 * if the msgid is empty (the header entry), don't add a string for
 * it. The same for some other entries like 'translator-credits' etc.
 * In such cases return NULL.
 */
function _get_sguid($entry, $uid) {
  // Get the string.
  $string = $entry['msgid'];
  if (isset($entry['msgid_plural'])) {
    $string .= "\0" . $entry['msgid_plural'];
  }

  // Don't add the header entry as a translatable string.
  // Don't add strings like 'translator-credits' etc. as translatable strings.
  if ($string == '')  return NULL;
  if (preg_match('/.*translator.*credit.*/', $string))  return NULL;

  // Get the context.
  $context = isset($entry['msgctxt']) ? $entry['msgctxt'] : '';

  // Get the $sguid of this string.
  $sguid = sha1($string . $context);

  // Make sure that such a string is stored in the DB.
  if (!btr_get_string($sguid)) {
    btr_insert('btr_strings')
      ->fields(array(
          'string' => $string,
          'context' => $context,
          'sguid' => $sguid,
          'uid' => $uid,
          'time' => date('Y-m-d H:i:s', REQUEST_TIME),
          'count' => 1,
        ))
      ->execute();
  }

  return $sguid;
}

/**
 * Add the given translation to the string, if it does not exist.
 * If it exists, just add a vote for the translation and set the
 * author, if the translation has no author.
 */
function _add_translation($sguid, $lng, $translation, $umail) {
  $tguid = sha1($translation . $lng . $sguid);

  // Check whether this translation exists.
  $query = 'SELECT * FROM {btr_translations} WHERE tguid = :tguid';
  $args = array(':tguid' => $tguid);
  $result = btr_query($query, $args)->fetch();

  if (!$result) {
    // Add the translation for this string.
    btr::translation_add($sguid, $lng, $translation);
  }
  else {
    // Add a vote for the translation.
    btr::vote_add($tguid);
    // Update the author of the translations.
    if (empty($result->umail) or $result->umail == 'admin@example.com') {
      btr_update('btr_translations')
        ->fields(array(
            'umail' => $umail,
            'time' => date('Y-m-d H:i:s', REQUEST_TIME),
          ))
        ->condition('tguid', $tguid)
        ->execute();
    }
  }
}