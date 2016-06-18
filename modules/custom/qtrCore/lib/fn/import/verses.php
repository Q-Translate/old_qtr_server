<?php
/**
 * @file
 * Function: import_verses()
 */

namespace QTranslate;
use \qtr;

/**
 * Import the verses of the Quran from the given xml data file.
 */
function import_verses($file) {
  $quran = simplexml_load_file($file) or die("Error: Cannot load xml file: $file\n");

  qtr::db_query('TRUNCATE qtr_verses')->execute();;
  foreach($quran->sura as $chapter) {
    foreach ($chapter->aya as $verse) {
      qtr::db_insert('qtr_verses')
        ->fields(array(
            'chapter_id' => $chapter['index'],
            'verse_nr' => $verse['index'],
            'verse_text' => $verse['text'],
          ))
        ->execute();
    }
  }
}
