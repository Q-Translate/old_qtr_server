<?php
/**
 * @file
 * qtr_qtr.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function qtr_qtr_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'qtranslator-admin'.
  $permissions['qtranslator-admin'] = array(
    'name' => 'qtranslator-admin',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'qtrUser',
  );

  // Exported permission: 'qtranslator-export'.
  $permissions['qtranslator-export'] = array(
    'name' => 'qtranslator-export',
    'roles' => array(
      'translator' => 'translator',
    ),
    'module' => 'qtrUser',
  );

  // Exported permission: 'qtranslator-flag'.
  $permissions['qtranslator-flag'] = array(
    'name' => 'qtranslator-flag',
    'roles' => array(
      'contributor' => 'contributor',
      'moderator' => 'moderator',
      'translator' => 'translator',
    ),
    'module' => 'qtrUser',
  );

  // Exported permission: 'qtranslator-import'.
  $permissions['qtranslator-import'] = array(
    'name' => 'qtranslator-import',
    'roles' => array(
      'translator' => 'translator',
    ),
    'module' => 'qtrUser',
  );

  // Exported permission: 'qtranslator-resolve'.
  $permissions['qtranslator-resolve'] = array(
    'name' => 'qtranslator-resolve',
    'roles' => array(
      'moderator' => 'moderator',
    ),
    'module' => 'qtrUser',
  );

  // Exported permission: 'qtranslator-stats'.
  $permissions['qtranslator-stats'] = array(
    'name' => 'qtranslator-stats',
    'roles' => array(
      'anonymous user' => 'anonymous user',
      'authenticated user' => 'authenticated user',
      'moderator' => 'moderator',
    ),
    'module' => 'qtrUser',
  );

  // Exported permission: 'qtranslator-suggest'.
  $permissions['qtranslator-suggest'] = array(
    'name' => 'qtranslator-suggest',
    'roles' => array(
      'contributor' => 'contributor',
      'moderator' => 'moderator',
      'translator' => 'translator',
    ),
    'module' => 'qtrUser',
  );

  // Exported permission: 'qtranslator-vote'.
  $permissions['qtranslator-vote'] = array(
    'name' => 'qtranslator-vote',
    'roles' => array(
      'contributor' => 'contributor',
      'moderator' => 'moderator',
      'translator' => 'translator',
    ),
    'module' => 'qtrUser',
  );

  return $permissions;
}
