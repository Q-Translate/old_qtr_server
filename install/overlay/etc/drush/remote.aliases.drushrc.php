<?php

/* uncomment and modify properly

$aliases['live'] = array (
  'root' => '/var/www/qtr',
  'uri' => 'http://qtranslate.org',

  'remote-host' => 'qtranslate.org',
  'remote-user' => 'root',
  'ssh-options' => '-p 2201 -i /root/.ssh/id_rsa',

  'path-aliases' => array (
    '%profile' => 'profiles/qtr_server',
    '%data' => '/var/www/data',
    '%pofiles' => '/var/www/PO_files',
    '%uploads' => '/var/www/uploads',
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
  'uri' => 'http://test.qtranslate.org',

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
