<?php
/**
 * @file
 * Return the latest translations in JSON format.
 */

function qtrCore_latest($lng ='en') {
  // Get the data from cache, if possible.
  $cache = cache_get("qtrCore_latest:$lng", 'cache_qtrCore');
  if (!empty($cache) && isset($cache->data) && !empty($cache->data)) {
    $latest = $cache->data;
  }
  else {
    // Get the latest translations from the database.
    $latest = qtr::translation_latest($lng);
    // Cache them for 2 hours.
    cache_set("qtrCore_latest:$lng", $latest, 'cache_qtrCore', strtotime('+2 hours'));
  }

  // Output latest translations in JSON format and exit.
  print drupal_json_encode($latest);
  exit();
}
