<?php
/**
 * @file
 * Function: cron_send_verses_for_review()
 */

namespace QTranslate;
use \qtr;

/**
 * Send by email a string for review to all the active users.
 */
function cron_send_verses_for_review() {

  // return true if we should NOT send a verse by email to the given account
  function _qtrCore_dont_send_email($account) {
    // skip admin, disabled accounts, and users that have never logged in
    if ($account->uid < 2)     return TRUE;
    if ($account->status != 1) return TRUE;
    if ($account->login == 0)  return TRUE;
    return FALSE;
  }

  $notifications = array();
  $accounts = entity_load('user');
  foreach ($accounts as $account) {
    if (_qtrCore_dont_send_email($account))  continue;

    // get a random vid
    $vid = rand(1, 6236);
    if (!$vid)  continue;

    // Get details of the verse and the translation.
    $verse = qtr::db_query('SELECT * FROM {qtr_verses} WHERE vid = :vid', [':vid' => $vid])->fetch();

    $message_params = array(
      'type' => 'verse-to-be-reviewed',
      'uid' => $account->uid,
      'username' => $account->name,
      'recipient' => $account->name .' <' . $account->mail . '>',
      'lng' => $account->translation_lng,
      'chapter_id' => $verse->cid,
      'verse_nr' => $verse->nr,
      'vid' => $vid,
    );
    $notifications[] = $message_params;
  }
  qtr::queue('notifications', $notifications);
}
