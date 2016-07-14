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
 *   Id of the user that is importing these translations.
 */
function import_translations($lng, $file, $uid = NULL, $notime = FALSE) {
  $handle = fopen($file, 'r') or die("Error: Cannot open file: $file\n");

  // Get the email of the author of the translation, etc.
  $uid = qtr::user_check($uid);
  $account = user_load($uid);
  $umail = ($uid==1 ?  $umail = '' : $account->init);
  $time = ($notime ? NULL : date('Y-m-d H:i:s', REQUEST_TIME));

  $vid = 0;
  while(!feof($handle)){
    $vid++;
    $translation = fgets($handle);
    $translation = trim($translation);
    $tguid = sha1($translation . $lng . $vid);
    $fields = array(
      'tguid' => $tguid,
      'vid' => $vid,
      'lng' => $lng,
      'translation' => $translation,
      'count' => 0,
      'umail' => $umail,
      'ulng' => $lng,
    );
    if ($time != NULL)  $fields['time'] = $time;

    // Add a new translation and a like for it.
    qtr::db_delete('qtr_translations')->condition('tguid', $tguid)->execute();
    qtr::db_insert('qtr_translations')->fields($fields)->execute();
    if ($uid > 1) qtr::like_add($tguid, $uid);
  }
  fclose($handle);
}
