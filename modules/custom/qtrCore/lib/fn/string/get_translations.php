<?php
/**
 * @file
 * Function: string_get_translations()
 */

namespace BTranslator;
use \qtr;

/**
 * Return a string and its translations.
 *
 * @param $sguid
 *   ID of the string.
 *
 * @param $lng
 *   Language of translations.
 *
 * @return
 *   array($string, $translations)
 */
function string_get_translations($sguid, $lng) {
  $query = qtr::db_select('qtr_strings', 's')
    ->fields('s', array('sguid'))
    ->where('s.sguid = :sguid', array(':sguid' => $sguid));
  $strings = qtr::string_details($query, $lng);

  $string = $strings[0]->string;
  $translations = array();
  foreach ($strings[0]->translations as $obj_translation) {
    $translations[] = $obj_translation->translation;
  }

  return array($string, $translations);
}
