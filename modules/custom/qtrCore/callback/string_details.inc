<?php
/**
 * @file
 * Callback for string_details.
 */

/**
 * Return string context, a list of related projects (where the string appears), etc.
 *
 * @param $sguid
 *   Id of the string.
 */
function qtrCore_string_details($sguid) {

  $details = [
    'context' => '',
    'projects' => [],
  ];

  // Get the context of this string.
  $query = "SELECT context FROM {qtr_strings} WHERE sguid = :sguid";
  $args = array(':sguid' => $sguid);
  $context = qtr::db_query($query, $args)->fetchField();
  $details['context'] = $context;

  // Get the projects where this string is used.
  $sql = "
    SELECT DISTINCT p.origin, p.project
    FROM {qtr_strings} s
    RIGHT JOIN {qtr_locations} l ON (l.sguid = s.sguid)
    RIGHT JOIN {qtr_templates} t ON (t.potid = l.potid)
    RIGHT JOIN {qtr_projects} p ON (p.pguid = t.pguid)
    WHERE s.sguid = :sguid
    ORDER BY p.origin, p.project
  ";
  $args = array(':sguid' => $sguid);
  $result = qtr::db_query($sql, $args)->fetchAll();
  foreach ($result as $row) {
    $project = $row->origin . '/' . $row->project;
    $details['projects'][] = $project;
  }

  // Return the string details.
  print drupal_json_encode($details);
  exit();
}
