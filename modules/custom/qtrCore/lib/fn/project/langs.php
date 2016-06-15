<?php
/**
 * @file
 * Definition of function project_langs()
 * which returns a list of languages of a project.
 */

namespace QTranslate;
use \qtr;

/**
 * Get a list of languages for the given origin/project.
 *
 * @param $origin
 *   The origin of the project.
 *
 * @param $project
 *   The name of the project.
 */
function project_langs($origin, $project) {
  $sql = "
      SELECT DISTINCT l.code, l.name
      FROM {qtr_templates} t
      JOIN {qtr_files} f ON (f.potid = t.potid)
      JOIN {qtr_languages} l ON (l.code = f.lng)
      WHERE t.pguid = :pguid
      ORDER by l.name
  ";
  $args = [ ':pguid' => sha1($origin . $project) ];
  $langs = qtr::db_query($sql, $args)->fetchAllKeyed();

  return $langs;
}
