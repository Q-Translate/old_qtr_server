<?php
/**
 * Description of the command 'qtr-vocabulary-get'.
 */
function _qtr_vocabulary_get_drush_command() {
  return array(
    'description' => "Export the terms and translations of a vocabulary.",
    'arguments' => array(
      'name' => 'The name of the vocabulary.',
      'lng' => 'The language of the vocabulary.',
    ),
    'options' => array(
      'format' => 'The output format: json | txt1 | txt2 | org. Default: json',
    ),
    'examples' => array(
      "drush qtrv-get ICT sq | python -m json.tool" => 'Get vocabulary ICT_sq in json format.',
      "drush qtrv-get ICT de --format=txt1 > ICT_sq.txt" => 'Get and save vocabulary ICT_de in text format.',
    ),
    'aliases' => array('qtrv-get'),
  );
}

/**
 * Help text for the command 'qtr-vocabulary-get'.
 */
function _qtr_vocabulary_get_drush_help() {
  return dt("Export the terms and translations of a vocabulary.");
}

/**
 * Callback function for the command 'qtr-vocabulary-get'.
 */
function drush_qtranslate_qtr_vocabulary_get($name, $lng) {
  // Get the vocabulary content.
  $result = qtr::vocabulary_get($name, $lng);

  // Get the format of the output.
  $format = drush_get_option('format', 'JSON');
  $format = strtoupper($format);
  if (!in_array($format, ['JSON', 'TXT1', 'TXT2', 'ORG']))  $format = 'JSON';

  // Output the result.
  switch ($format) {
    default:
    case 'JSON':
      $output = drupal_json_encode($result);
      print $output;
      break;

    case 'TXT1':
      $output = '';
      foreach ($result as $string => $translations) {
        $output .= $string . ' <==> ' . implode(' / ', $translations) . "\n";
      }
      print $output;
      break;

    case 'TXT2':
      $output = '';
      foreach ($result as $string => $translations) {
        $output .= '  + ' . $string . "\n";
        foreach ($translations as $translation) {
          $output .= '      - ' . $translation . "\n";
        }
        $output .= "\n";
      }
      print $output;
      break;

    case 'ORG':
      $output = '';
      foreach ($result as $string => $translations) {
        $output .= '  | *' . $string . '* | -- | ';
        $output .= implode('; ', $translations);
        $output .= " |\n";
      }
      print $output;
      break;
  }
}
