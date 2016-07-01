<?php
/**
 * @file
 * Function: verse_get_translations()
 */

namespace QTranslate;
use \qtr;

/**
 * Return a verse and its translations.
 *
 * @param $vid
 *   ID of the verse.
 *
 * @param $lng
 *   Language of translations.
 *
 * @return
 *   array($verse, $translations)
 */
function verse_get_translations($vid, $lng) {
  $query = qtr::db_select('qtr_verses', 'v')
    ->fields('v', array('vid'))
    ->where('v.vid = :vid', array(':vid' => $vid));
  $verses = qtr::verse_details($query, $lng);

  $verse = $verses[0]->verse;
  $translations = array();
  foreach ($verses[0]->translations as $obj_translation) {
    $translations[] = $obj_translation->translation;
  }

  return array($verse, $translations);
}
