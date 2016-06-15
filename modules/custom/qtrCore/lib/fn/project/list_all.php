<?php
/**
 * @file
 * Definition of function project_list_all().
 */

namespace QTranslate;
use \qtr;

/**
 * Return a full list of all the imported projects and languages.
 */
function project_list_all() {
  $sql = "
      SELECT DISTINCT p.origin, p.project, f.lng
      FROM {qtr_projects} p
      JOIN {qtr_templates} t ON (t.pguid = p.pguid)
      JOIN {qtr_files} f ON (f.potid = t.potid)
      ORDER by p.origin, p.project
  ";
  $result = qtr::db_query($sql);

  $project_list = array();
  foreach ($result as $rec) {
    $project_list[$rec->origin][$rec->project][] = $rec->lng;
  }

  return $project_list;
}
