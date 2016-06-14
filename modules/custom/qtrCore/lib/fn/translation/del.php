<?php
/**
 * @file
 * Function: translation_del()
 */

namespace BTranslator;
use \qtr;

/**
 * Delete the translation with the given id and any related votes.
 *
 * @param $tguid
 *   ID of the translation.
 *
 * @param $notify
 *   Notify the author and voters of the deleted translation.
 *
 * @param $uid
 *   Id of the user that is deleting the translation.
 */
function translation_del($tguid, $notify = TRUE, $uid = NULL) {
  // Before deleting, get the author, voters, string and translation
  // (for notifications).
  $author = qtr::db_query(
    'SELECT u.uid, u.name, u.umail
     FROM {qtr_translations} t
     JOIN {qtr_users} u ON (u.umail = t.umail AND u.ulng = t.ulng)
     WHERE t.tguid = :tguid',
    array(':tguid' => $tguid))
    ->fetchObject();
  $voters = qtr::db_query(
    'SELECT u.uid, u.name, u.umail
     FROM {qtr_votes} v
     JOIN {qtr_users} u ON (u.umail = v.umail AND u.ulng = v.ulng)
     WHERE v.tguid = :tguid',
    array(':tguid' => $tguid))
    ->fetchAll();
  $sguid = qtr::db_query(
    'SELECT sguid FROM {qtr_translations} WHERE tguid = :tguid',
    [':tguid' => $tguid]
  )->fetchField();
  $string = qtr::string_get($sguid);
  $translation = qtr::translation_get($tguid);

  // Get the mail and lng of the user that is deleting the translation.
  $uid = qtr::user_check($uid);
  $account = user_load($uid);
  $umail = $account->init;    // email used for registration
  $ulng = $account->translation_lng;

  // Check that the current user has the right to delete translations.
  $is_own = ($umail == $author->umail);
  if (!$is_own and ($uid != 1)
    and !user_access('qtranslator-resolve', $account)
    and !qtr::user_has_project_role('admin', $sguid)
    and !qtr::user_has_project_role('moderator', $sguid))
    {
      $msg = t('You are not allowed to delete this translation!');
      qtr::messages($msg, 'error');
      return;
    }

  // Copy to the trash table the translation that will be deleted.
  $query = qtr::db_select('qtr_translations', 't')
    ->fields('t', array('sguid', 'lng', 'translation', 'tguid', 'count', 'umail', 'ulng', 'time', 'active'))
    ->condition('tguid', $tguid);
  $query->addExpression(':d_umail', 'd_umail', array(':d_umail' => $umail));
  $query->addExpression(':d_ulng', 'd_ulng', array(':d_ulng' => $ulng));
  $query->addExpression('NOW()', 'd_time');
  qtr::db_insert('qtr_translations_trash')->from($query)->execute();

  // Copy to the trash table the votes that will be deleted.
  $query = qtr::db_select('qtr_votes', 'v')
    ->fields('v', array('vid', 'tguid', 'umail', 'ulng', 'time', 'active'))
    ->condition('tguid', $tguid);
  $query->addExpression('NOW()', 'd_time');
  qtr::db_insert('qtr_votes_trash')->from($query)->execute();

  // Delete the translation and any votes related to it.
  qtr::db_delete('qtr_translations')->condition('tguid', $tguid)->execute();
  qtr::db_delete('qtr_votes')->condition('tguid', $tguid)->execute();

  // Notify the author of a translation and its voters
  // that it has been deleted.
  if ($notify) {
    _notify_voters_on_translation_del($sguid, $tguid, $string, $translation, $author, $voters);
  }
}

/**
 * Notify the author of a translation and its voters
 * that it has been deleted.
 */
function _notify_voters_on_translation_del($sguid, $tguid, $string, $translation, $author, $voters) {

  $notifications = array();

  // Notify the author of the translation about the deletion.
  if ($author->uid) {
    $notification = array(
      'type' => 'notify-author-on-translation-deletion',
      'uid' => $author->uid,
      'username' => $author->name,
      'recipient' => $author->name . ' <' . $author->umail . '>',
      'sguid' => $sguid,
      'string' => $string,
      'translation' => $translation,
    );
    $notifications[] = $notification;
  }

  // Notify the voters of the translation as well.
  foreach ($voters as $voter) {
    if (!$voter->uid)  continue;
    if ($voter->name == $author->name)  continue;  // don't send a second message to the author
    $notification = array(
      'type' => 'notify-voter-on-translation-deletion',
      'uid' => $voter->uid,
      'username' => $voter->name,
      'recipient' => $voter->name . ' <' . $voter->umail . '>',
      'sguid' => $sguid,
      'string' => $string,
      'translation' => $translation,
    );
    $notifications[] = $notification;
  }

  qtr::queue('notifications', $notifications);
}
