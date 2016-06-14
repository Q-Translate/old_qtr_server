<?php
/**
 * @file
 * Function: string_get()
 */

namespace BTranslator;
use \qtr;

/**
 * Get a string from its ID.
 */
function string_get($sguid) {
  $string = qtr::db_query(
    'SELECT string FROM {qtr_strings} WHERE sguid = :sguid',
    array(':sguid' => $sguid)
  )->fetchField();
  return $string;
}
