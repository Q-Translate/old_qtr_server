<?php
/**
 * @file
 * Function: project_exists()
 */

namespace QTranslate;
use \qtr;

/**
 * Check weather the given origin/project exists.
 *
 * @param $origin
 *   The origin of the project.
 *
 * @param $project
 *   The name of the project.
 *
 * @return
 *   TRUE if such a project exists, otherwise FALSE.
 */
function project_exists($origin, $project) {
  $project = qtr::db_query(
    'SELECT project FROM {qtr_projects}
     WHERE BINARY origin = :origin AND BINARY project = :project',
    array(
      ':origin' => $origin,
      ':project' => $project,
    ))
    ->fetchField();

  return ($project ? TRUE : FALSE);
}
