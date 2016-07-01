<?php
/**
 * @file
 * Function: translation_del()
 */

namespace QTranslate;
use \qtr;

/**
 * Delete the translation with the given id and any related likes.
 *
 * @param $tguid
 *   ID of the translation.
 *
 * @param $notify
 *   Notify the author and likers of the deleted translation.
 *
 * @param $uid
 *   Id of the user that is deleting the translation.
 */
function translation_del($tguid, $notify = TRUE, $uid = NULL) {
  // Before deleting, get the author, likers, verse and translation
  // (for notifications).
  $author = qtr::db_query(
    'SELECT u.uid, u.name, u.umail
     FROM {qtr_translations} t
     JOIN {qtr_users} u ON (u.umail = t.umail AND u.ulng = t.ulng)
     WHERE t.tguid = :tguid',
    array(':tguid' => $tguid))
    ->fetchObject();
  $users = qtr::db_query(
    'SELECT u.uid, u.name, u.umail
     FROM {qtr_likes} l
     JOIN {qtr_users} u ON (u.umail = l.umail AND u.ulng = l.ulng)
     WHERE l.tguid = :tguid',
    array(':tguid' => $tguid))
    ->fetchAll();
  $vid = qtr::db_query(
    'SELECT vid FROM {qtr_translations} WHERE tguid = :tguid',
    [':tguid' => $tguid]
  )->fetchField();
  $verse = qtr::verse_get($vid);
  $translation = qtr::translation_get($tguid);

  // Get the mail and lng of the user that is deleting the translation.
  $uid = qtr::user_check($uid);
  $account = user_load($uid);
  $umail = $account->init;    // email used for registration
  $ulng = $account->translation_lng;

  // Check that the current user has the right to delete translations.
  $is_own = ($umail == $author->umail);
  if (!$is_own and ($uid != 1)
    and !user_access('qtranslate-resolve', $account)
    and !qtr::user_has_project_role('admin', $vid)
    and !qtr::user_has_project_role('moderator', $vid))
    {
      $msg = t('You are not allowed to delete this translation!');
      qtr::messages($msg, 'error');
      return;
    }

  // Copy to the trash table the translation that will be deleted.
  $query = qtr::db_select('qtr_translations', 't')
    ->fields('t', array('tguid', 'vid', 'lng', 'translation', 'count', 'umail', 'ulng', 'time', 'active'))
    ->condition('tguid', $tguid);
  $query->addExpression(':d_umail', 'd_umail', array(':d_umail' => $umail));
  $query->addExpression(':d_ulng', 'd_ulng', array(':d_ulng' => $ulng));
  $query->addExpression('NOW()', 'd_time');
  qtr::db_insert('qtr_translations_trash')->from($query)->execute();

  // Copy to the trash table the likes that will be deleted.
  $query = qtr::db_select('qtr_likes', 'l')
    ->fields('l', array('lid', 'tguid', 'umail', 'ulng', 'time', 'active'))
    ->condition('tguid', $tguid);
  $query->addExpression('NOW()', 'd_time');
  qtr::db_insert('qtr_likes_trash')->from($query)->execute();

  // Delete the translation and any likes related to it.
  qtr::db_delete('qtr_translations')->condition('tguid', $tguid)->execute();
  qtr::db_delete('qtr_likes')->condition('tguid', $tguid)->execute();

  // Notify the author of a translation and its users
  // that it has been deleted.
  if ($notify) {
    _notify_users_on_translation_del($vid, $tguid, $verse, $translation, $author, $users);
  }
}

/**
 * Notify the author of a translation and its users
 * that it has been deleted.
 */
function _notify_users_on_translation_del($vid, $tguid, $verse, $translation, $author, $users) {

  $notifications = array();

  // Notify the author of the translation about the deletion.
  if ($author->uid) {
    $notification = array(
      'type' => 'notify-author-on-translation-deletion',
      'uid' => $author->uid,
      'username' => $author->name,
      'recipient' => $author->name . ' <' . $author->umail . '>',
      'vid' => $vid,
      'verse' => $verse,
      'translation' => $translation,
    );
    $notifications[] = $notification;
  }

  // Notify the users that have liked the translation as well.
  foreach ($users as $user) {
    if (!$user->uid)  continue;
    if ($user->name == $author->name)  continue;  // don't send a second message to the author
    $notification = array(
      'type' => 'notify-user-on-translation-deletion',
      'uid' => $user->uid,
      'username' => $user->name,
      'recipient' => $user->name . ' <' . $user->umail . '>',
      'vid' => $vid,
      'verse' => $verse,
      'translation' => $translation,
    );
    $notifications[] = $notification;
  }

  qtr::queue('notifications', $notifications);
}
