<?php
/**
 * @file
 * qtr_hybridauth.features.fe_block_settings.inc
 */

/**
 * Implements hook_default_fe_block_settings().
 */
function qtr_hybridauth_default_fe_block_settings() {
  $export = array();

  $export['version'] = '2.0';

  $export['hybridauth-hybridauth'] = array(
    'cache' => 8,
    'custom' => 0,
    'delta' => 'hybridauth',
    'module' => 'hybridauth',
    'node_types' => array(),
    'pages' => '',
    'roles' => array(),
    'themes' => array(
      'qtr_server' => array(
        'region' => '',
        'status' => 0,
        'theme' => 'qtr_server',
        'weight' => 0,
      ),
      'seven' => array(
        'region' => '',
        'status' => 0,
        'theme' => 'seven',
        'weight' => 0,
      ),
    ),
    'title' => '',
    'visibility' => 0,
  );

  return $export;
}
