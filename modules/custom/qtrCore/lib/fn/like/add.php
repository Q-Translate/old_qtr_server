<?php
namespace QTranslate;
use \qtr;

/**
 * Add a like for the given translation from the given user.
 * Make sure that any previous like is cleaned first
 * (don't allow multiple likes for the same translation).
 *
 * @param $tguid
 *   ID of the translation.
 *
 * @param $uid (optional)
 *   ID of the user.
 *
 * @return
 *   ID of the new like, or NULL.
 */
function like_add($tguid, $uid = NULL) {
  // Don't add a like for anonymous and admin users.
  $uid = qtr::user_check($uid);
  if ($uid == 1)  return NULL;

  // Fetch the translation details from the DB.
  $sql = 'SELECT * FROM {qtr_translations} WHERE tguid = :tguid';
  $trans = qtr::db_query($sql, [':tguid' => $tguid])->fetchObject();

  // If there is no such translation, return NULL.
  if (empty($trans)) {
    $msg = t('The given translation does not exist.');
    qtr::messages($msg, 'error');
    return NULL;
  }

  // Get the mail and lng of the user.
  $account = user_load($uid);
  $umail = $account->init;    // email used for registration
  $ulng = $account->translation_lng;

  // Make sure that the language of the user matches the language of the translation.
  if ($ulng != $trans->lng and !user_access('qtranslate-admin', $account)) {
    $msg = t('You cannot like the translations of language <strong>!lng</strong>', ['!lng' => $trans->lng]);
    qtr::messages($msg, 'error');
    return NULL;
  }

  // Clean any previous like.
  $nr = _like_del_previous($tguid, $umail, $trans->vid, $trans->lng);

  // Add the like.
  $vid = qtr::db_insert('qtr_likes')
    ->fields([
        'tguid' => $tguid,
        'umail' => $umail,
        'ulng' => $ulng,
        'time' => date('Y-m-d H:i:s', REQUEST_TIME),
      ])
    ->execute();

  // Update like count of the translation.
  $sql = 'SELECT COUNT(*) FROM {qtr_likes} WHERE tguid = :tguid';
  $count = qtr::db_query($sql, [':tguid' => $tguid])->fetchField();
  qtr::db_update('qtr_translations')
    ->fields(['count' => $count])
    ->condition('tguid', $tguid)
    ->execute();

  return $vid;
}

/**
 * Clean any previous like by this user for this translation.
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
