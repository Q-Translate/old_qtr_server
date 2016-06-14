<?php
/*
  For more info see:
    drush help site-alias
    drush topic docs-aliases

  See also:
    drush help rsync
    drush help sql-sync
 */

$aliases['qtr'] = array (
  'root' => '/var/www/qtr',
  'uri' => 'http://qtr.example.org',
  'path-aliases' => array (
    '%profile' => 'profiles/qtr_server',
    '%data' => '/var/www/data',
    '%po_files' => '/var/www/PO_files',
    '%exports' => '/var/www/exports',
    '%downloads' => '/var/www/downloads',
  ),
);

// $aliases['qtr_dev'] = array (
//   'parent' => '@qtr',
//   'root' => '/var/www/qtr_dev',
//   'uri' => 'http://dev.qtr.example.org',
// );
