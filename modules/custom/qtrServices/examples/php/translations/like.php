<?php
$path = dirname(dirname(__FILE__));
include_once($path . '/config.php');
include_once($path . '/http_request.php');
include_once($path . '/get_access_token.php');

// Get a random translated string.
$url = $base_url . '/api/translations/translated?lng=sq';
$result = http_request($url);

// Get the vid and the tguid of the first translation.
$vid = $result['string']['vid'];
$tguid = $result['string']['translations'][0]['tguid'];

// Get an access  token.
$access_token = get_access_token($auth);

// POST api/translations/like
$url = $base_url . '/api/translations/like';
$options = array(
  'method' => 'POST',
  'data' => array('tguid' => $tguid),
  'headers' => array(
    'Content-type' => 'application/x-www-form-urlencoded',
    'Authorization' => 'Bearer ' . $access_token,
  ),
);
$result = http_request($url, $options);

// Retrive the string and check that the translation has been liked.
$url = $base_url . "/api/translations/$vid?lng=sq";
$result = http_request($url);

// POST api/translations/del_like
$url = $base_url . '/api/translations/del_like';
$options = array(
  'method' => 'POST',
  'data' => array('tguid' => $tguid),
  'headers' => array(
    'Content-type' => 'application/x-www-form-urlencoded',
    'Authorization' => 'Bearer ' . $access_token,
  ),
);
$result = http_request($url, $options);
