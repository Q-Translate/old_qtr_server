<?php
/**
 * @file
 * qtr_layout.box.inc
 */

/**
 * Implements hook_default_box().
 */
function qtr_layout_default_box() {
  $export = array();

  $box = new stdClass();
  $box->disabled = FALSE; /* Edit this to true to make a default box disabled initially */
  $box->api_version = 1;
  $box->delta = 'boxabout';
  $box->plugin_key = 'simple';
  $box->title = '';
  $box->description = 'About Q-Translate';
  $box->options = array(
    'body' => array(
      'value' => '<p><strong><a href="http://info.qtranslate.org" target="_blank">Q-Translate</a></strong> helps to get feedback about the translations of programs from a wide crowd of people and users. This feedback can be in terms of likes for the best translation (when there are more than one translations for the same string), or it can be a new alternative translation (for an existing translation), or it can be a new translation suggestion (for a string that is not translated yet).</p>',
      'format' => 'full_html',
    ),
    'additional_classes' => '',
  );
  $export['boxabout'] = $box;

  return $export;
}