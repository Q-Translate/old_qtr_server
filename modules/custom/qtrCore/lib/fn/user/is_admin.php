<?php
/**
 * @file
 * Definition of function user_is_admin().
 */

namespace QTranslate;

/**
 * Return TRUE if the user can administrate.
 */
function user_is_admin($lng = NULL, $uid = NULL) {
  // Get the user account.
  if ($uid === NULL)  $uid = $GLOBALS['user']->uid;
  $account = user_load($uid);

  // If user has global admin permission,
  // he can administrate this project as well.
  if (user_access('qtranslate-admin', $account))  return TRUE;

  // Check that the project language matches translation_lng of the user.
  if ($lng !== NULL and $lng != $account->translation_lng) return FALSE;

  // Otherwise he cannot administrate.
  return FALSE;
}
