<?php
namespace QTranslate;
use \qtr;

/**
 * Clean any previous like by this user for this translation.
 *
 * This depends on the voting mode option (set by the admin).
 * If the voting mode is 'single', then the user can select
 * only one translation for a given string (at most one like
 * per string).
 * If the voting mode is 'multiple', then the user can approve
 * several translations for a string (at most one like per
 * translation).
 *
 * @param $tguid
 *   ID of the translation.
 *
 * @param $umail
 *   The mail of the user.
 *
 * @param $vid
 *   ID of the source string.
 *
 * @param $lng
 *   Language code of the translation.
 *
 * @return
 *   Number of previous likes that were deleted.
 *   (Normally should be 0, but can also be 1. If it is >1,
 *   something must be wrong.)
 */
function _like_del_previous($tguid, $umail, $vid, $lng) {
  // Get the voting mode.
  $voting_mode = variable_get('qtr_voting_mode', 'single');

  $arr_tguid = array();
  if ($voting_mode == 'multiple') {
    $arr_tguid = array($tguid);
  }
  else { // ($voting_mode == 'single')
    // Get the other sibling translations (translations of the same
    // string and the same language) which the user has liked.
    $arr_tguid = qtr::db_query(
      'SELECT DISTINCT t.tguid
       FROM {qtr_translations} t
       LEFT JOIN {qtr_likes} l ON (l.tguid = t.tguid)
       WHERE t.vid = :vid
         AND t.lng = :lng
         AND l.umail = :umail
         AND l.ulng = :ulng',
       array(
        ':vid' => $vid,
        ':lng' => $lng,
        ':umail' => $umail,
        ':ulng' => $lng,
      ))
      ->fetchCol();
  }

  if (empty($arr_tguid))  return 0;

  // Insert to the trash table the likes that will be removed.
  $query = qtr::db_select('qtr_likes', 'l')
    ->fields('l', array('lid', 'tguid', 'umail', 'ulng', 'time', 'active'))
    ->condition('umail', $umail)
    ->condition('ulng', $lng)
    ->condition('tguid', $arr_tguid, 'IN');
  $query->addExpression('NOW()', 'd_time');
  qtr::db_insert('qtr_likes_trash')->from($query)->execute();

  // Remove any likes by the user for each translation in $arr_tguid.
  $num_deleted = qtr::db_delete('qtr_likes')
    ->condition('umail', $umail)
    ->condition('ulng', $lng)
    ->condition('tguid', $arr_tguid, 'IN')
    ->execute();

  // Decrement the like count for each translation in $arr_tguid.
  $num_updated = qtr::db_update('qtr_translations')
    ->expression('count', 'count - 1')
    ->condition('tguid', $arr_tguid, 'IN')
    ->execute();

  return $num_deleted;
}
