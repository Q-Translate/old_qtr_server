<?php
namespace QTranslate;
use \qtr;

/**
 * Add a vote for the given translation from the current user.
 * Make sure that any previous vote is cleaned first
 * (don't allow multiple votes for the same translation).
 *
 * @param $tguid
 *   ID of the translation.
 *
 * @param $uid (optional)
 *   ID of the user.
 *
 * @return
 *   ID of the new vote, or NULL.
 */
function vote_add($tguid, $uid = NULL) {
  // Don't add a vote for anonymous and admin users.
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
    $msg = t('You cannot vote the translations of language <strong>!lng</strong>', ['!lng' => $trans->lng]);
    qtr::messages($msg, 'error');
    return NULL;
  }

  // Clean any previous vote.
  include_once(dirname(__FILE__) . '/del_previous.php');
  $nr = _vote_del_previous($tguid, $umail, $trans->sguid, $trans->lng);

  // Add the vote.
  $vid = qtr::db_insert('qtr_votes')
    ->fields([
        'tguid' => $tguid,
        'umail' => $umail,
        'ulng' => $ulng,
        'time' => date('Y-m-d H:i:s', REQUEST_TIME),
      ])
    ->execute();

  // Update vote count of the translation.
  $sql = 'SELECT COUNT(*) FROM {qtr_votes} WHERE tguid = :tguid';
  $count = qtr::db_query($sql, [':tguid' => $tguid])->fetchField();
  qtr::db_update('qtr_translations')
    ->fields(['count' => $count])
    ->condition('tguid', $tguid)
    ->execute();

  return $vid;
}
