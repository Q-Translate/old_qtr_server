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

  return;  // do nothing for the time being

  // return true if we should NOT send a verse by email to the given account
  function _qtrCore_dont_send_email($account) {
    // skip admin, disabled accounts, and users that have never logged in
    if ($account->uid < 2 or $account->status != 1 or $account->login == 0) {
      return TRUE;
    }

    // otherwise send email
    return FALSE;
  }

  $notifications = array();
  $accounts = entity_load('user');
  foreach ($accounts as $account) {
    if (_qtrCore_dont_send_email($account))  continue;

    // get a random vid
    //$vid = qtr::vid_get_random($account->uid, array($project));
    if (!$vid)  continue;

    $message_params = array(
      'type' => 'string-to-be-reviewed',
      'uid' => $account->uid,
      'vid' => $vid,
      'username' => $account->name,
      'recipient' => $account->name .' <' . $account->mail . '>',
    );
    $notifications[] = $message_params;
  }
  qtr::queue('notifications', $notifications);
}
