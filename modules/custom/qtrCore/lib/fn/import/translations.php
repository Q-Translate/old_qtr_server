<?php
/**
 * @file
 * Function: import_translations()
 */

namespace QTranslate;
use \qtr;

/**
 * Import translations of the Quran from the given text data file.
 *
 * @param $lng
 *   The language (code) of the translation.
 *
 * @param $file
 *   The text file of the translations,
 *   having each translation in a separate line.
 *
 * @param $uid
 *   Id of the user that is adding the string.
 */
function import_translations($lng, $file, $uid = NULL) {
  $handle = fopen($file, 'r') or die("Error: Cannot open file: $file\n");

  // Get the email of the author of the translation.
  $uid = qtr::user_check($uid);
  $account = user_load($uid);
  $umail = ($uid==1 ?  $umail = '' : $account->init);

  $vid = 0;
  while(!feof($handle)){
    $vid++;
    $translation = fgets($handle);
    $tguid = sha1($translation . $lng . $vid);
    qtr::db_delete('qtr_translations')
      ->condition('tguid', $tguid)
      ->execute();
    qtr::db_insert('qtr_translations')
      ->fields(array(
          'tguid' => $tguid,
          'vid' => $vid,
          'lng' => $lng,
          'translation' => $translation,
          'count' => 0,
          'umail' => $umail,
          'ulng' => $lng,
          'time' => date('Y-m-d H:i:s', REQUEST_TIME),
        ))
      ->execute();
    qtr::like_add($tguid, $uid);
  }
  fclose($handle);
}