<?php
/**
 * @file
 * Function: translation_latest()
 */

namespace BTranslator;
use \qtr;

/**
 * Get and return an array of the latest translation suggestions,
 * submitted between the last midnight and the midnight before.
 * If $origin and $project are given, then they will be used
 * to filter only the translations that belong to them.
 * The fields that are returned are:
 *   origin, project, sguid, string, lng, translation, tguid, time, name, umail
 */
function translation_latest($lng, $origin =NULL, $project =NULL) {

  $get_latest_translations = "
    SELECT p.origin, p.project,
           s.sguid, s.string,
           t.lng, t.translation, t.tguid, t.time,
           u.name, u.umail
    FROM {qtr_translations} t
    LEFT JOIN {qtr_strings} s ON (s.sguid = t.sguid)
    LEFT JOIN {qtr_users} u ON (u.umail = t.umail AND u.ulng = t.ulng)
    LEFT JOIN {qtr_locations} l ON (l.sguid = s.sguid)
    LEFT JOIN {qtr_templates} tpl ON (tpl.potid = l.potid)
    LEFT JOIN {qtr_projects} p ON (p.pguid = tpl.pguid)
    WHERE t.umail != '' AND t.lng = :lng AND t.time > :from_date
    ";

  $args = array(
    ':lng' => $lng,
    ':from_date' => date('Y-m-d', strtotime("-1 day")),
  );

  if ( ! empty($origin) ) {
    // filter by origin
    $get_latest_translations .= " AND p.origin = :origin";
    $args[':origin'] = $origin;

    if ( ! empty($project) ) {
      // filter also by project
      $get_latest_translations .= " AND p.project = :project";
      $args[':project'] = $project;
    }
  }

  // display the latest first
  $get_latest_translations .= "\n    ORDER BY t.time DESC";

  // run the query and get the translations
  $translations = qtr::db_query($get_latest_translations, $args)->fetchAll();

  return $translations;
}
