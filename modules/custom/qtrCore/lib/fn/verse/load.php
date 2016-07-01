<?php
/**
 * @file
 * Function: verse_load()
 */

namespace QTranslate;
use \qtr;

/**
 * Get details for a list of verses.
 *
 * @param $arr_vid
 *   List of verse IDs to be loaded.
 *
 * @param $lng
 *   Language of translations.
 *
 * @return
 *   An array of verses, translations and likes, where each verse
 *   is an associative array, with translations and likes as nested
 *   associative arrays.
 */
function verse_load($arr_vid, $lng) {
  $query = qtr::db_select('qtr_verses', 'v')
    ->fields('v', array('vid'))
    ->condition('v.vid', $arr_vid, 'IN');

  // Get alternative langs from the preferences of the user.
  $alternative_langs = array();
  global $user;
  $user = user_load($user->uid);
  if (isset($user->auxiliary_languages) and is_array($user->auxiliary_languages)) {
    $alternative_langs = $user->auxiliary_languages;
  }

  return qtr::verse_details($query, $lng, $alternative_langs);
}
