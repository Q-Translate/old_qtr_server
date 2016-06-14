<?php
/**
 * @file
 * Function: cron_send_strings_for_review()
 */

namespace BTranslator;
use \qtr;

/**
 * Send by email a string for review to all the active users.
 */
function cron_send_strings_for_review() {

  // return true if we should NOT send a string by email to the given account
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

    // Get a random project from user preferences.
    if (empty($account->subscribed_projects))  continue;
    $idx = rand(0, sizeof($account->subscribed_projects) - 1);
    $project = $account->subscribed_projects[$idx];

    // get a sguid according to the user preferencies
    module_load_include('inc', 'qtrCore', 'includes/get_sguid');
    $sguid = qtr::sguid_get_random($account->uid, array($project));
    if (!$sguid)  continue;

    $message_params = array(
      'type' => 'string-to-be-reviewed',
      'uid' => $account->uid,
      'sguid' => $sguid,
      'project' => $project,
      'username' => $account->name,
      'recipient' => $account->name .' <' . $account->mail . '>',
    );
    $notifications[] = $message_params;
  }
  qtr::queue('notifications', $notifications);
}
