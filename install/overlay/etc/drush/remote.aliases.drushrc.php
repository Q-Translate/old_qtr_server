<?php

/* uncomment and modify properly

$aliases['live'] = array (
  'root' => '/var/www/qtr',
  'uri' => 'http://qtr.example.org',

  'remote-host' => 'qtr.example.org',
  'remote-user' => 'root',
  'ssh-options' => '-p 2201 -i /root/.ssh/id_rsa',

  'path-aliases' => array (
    '%profile' => 'profiles/qtr_server',
    '%data' => '/var/www/data',
    '%pofiles' => '/var/www/PO_files',
    '%exports' => '/var/www/exports',
    '%downloads' => '/var/www/downloads',
  ),

  'command-specific' => array (
    'sql-sync' => array (
      'simulate' => '1',
    ),
    'rsync' => array (
      'simulate' => '1',
    ),
  ),
);

$aliases['test'] = array (
  'parent' => '@live',
  'root' => '/var/www/qtr_test',
  'uri' => 'http://test.qtr.example.org',

  'command-specific' => array (
    'sql-sync' => array (
      'simulate' => '0',
    ),
    'rsync' => array (
      'simulate' => '0',
    ),
  ),
);

*/