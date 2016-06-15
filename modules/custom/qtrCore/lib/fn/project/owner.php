<?php
/**
 * @file
 * Function: project_owner()
 */

namespace QTranslate;
use \qtr;

/**
 * Get the uid of the project owner.
 *
 * @param $origin
 *   The origin of the project.
 *
 * @param $project
 *   The name of the project.
 *
 * @return
 *   The uid of the project owner.
 */
function project_owner($origin, $project) {
  $uid = qtr::db_query(
    'SELECT uid FROM {qtr_projects} WHERE origin = :origin AND project = :project',
    [
      ':origin' => $origin,
      ':project' => $project,
    ])
    ->fetchField();

  return $uid;
}
