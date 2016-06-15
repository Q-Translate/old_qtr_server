#!/usr/bin/drush php-script
<?php

$url = 'https://dev.qtranslate.org/api/report/statistics.json';
$options = array(
  'method' => 'POST',
  'data' => 'lng=sq',
);
$response = drupal_http_request($url, $options);
print_r($response);
