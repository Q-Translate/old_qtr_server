<?php
/**
 * @file
 * Function project_add_admin().
 */

namespace QTranslate;
use \qtr;

/**
 * Add the user $uid as admin of project ($origin, $project, $lng).
 */
function project_add_admin($origin, $project, $lng, $uid) {
  // Don't add anonymous and admin users.
  if ($uid == 0 or $uid == 1)  return;

  // Get email of the user.
  $account = user_load($uid);
  $email = $account->init;

  // Delete it first, if he exists.
  qtr::db_delete('qtr_user_project_roles')
    ->condition('umail', $email)
    ->condition('ulng', $lng)
    ->condition('role', 'admin')
    ->execute();

  // Add as admin.
  qtr::db_insert('qtr_user_project_roles')
    ->fields([
        'umail' => $email,
        'ulng' => $lng,
        'pguid' => sha1($origin . $project),
        'role' => 'admin',
      ])
    ->execute();
}
