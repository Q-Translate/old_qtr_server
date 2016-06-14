<?php
/*
  For more info see:
    drush help site-alias
    drush topic docs-aliases

  See also:
    drush help rsync
    drush help sql-sync
 */

$aliases['qcl'] = array (
  'root' => '/var/www/qcl',
  'uri' => 'http://example.org',
  'path-aliases' => array (
    '%profile' => 'profiles/qtr_client',
    '%downloads' => '/var/www/downloads',
  ),
);

// $aliases['qcl_dev'] = array (
//   'parent' => '@qcl',
//   'root' => '/var/www/qcl_dev',
//   'uri' => 'http://dev.example.org',
// );
