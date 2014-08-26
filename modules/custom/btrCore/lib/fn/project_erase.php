<?php
/**
 * @file
 * Definition of function project_erase() which is used for deleting projects.
 */

namespace BTranslator;

/**
 * Delete everything related to the given origin and project.
 *
 * It will delete templates, locations, files, snapshots, diffs
 * and the projects itself (but not the strings, translations, etc.)
 * If no project is given, then all the projects of the given origin
 * will be deleted. If the origin is NULL, then all the projects
 * of the given name (from any origin) will be deleted.
 *
 * @param $origin
 *   The origin of the project to be deleted.
 *
 * @param $project
 *   The name of the project to be deleted.
 *
 * @param $purge
 *   If true, then snapshots and diffs are deleted as well.
 */
function project_erase($origin =NULL, $project =NULL, $purge =TRUE) {
  // Get a list of matching projects.
  $pguid_list = _get_projects($origin, $project);
  if (!$pguid_list)  return;

  // Get a list of templates related to the project(s).
  $get_potid_list = "
    SELECT potid FROM {btr_templates} t
    LEFT JOIN {btr_projects} p ON (t.pguid = p.pguid)
    WHERE p.pguid IN (:pguid_list)";
  $args = array(':pguid_list' => $pguid_list);
  $potid_list = btr_query($get_potid_list, $args)->fetchCol();

  // Erase the data of each template.
  foreach ($potid_list as $potid) {
    _erase_template($potid);
  }

  if ($purge) {
    // Delete the diffs and snapshots of each project.
    foreach ($pguid_list as $pguid) {
      btr_delete('btr_diffs')->condition('pguid', $pguid)->execute();
      btr_delete('btr_snapshots')->condition('pguid', $pguid)->execute();
    }

    // Delete the projects themselves.
    btr_delete('btr_projects')
      ->condition('pguid', $pguid_list, 'IN')
      ->execute();
  }
}

/**
 * Return a list of project IDs (pguid) for the given $origin and $project
 * (if one of them is NULL, there may be more than one matching project)
 */
function _get_projects($origin, $project) {
  // The parameters should not be both NULL.
  if ($origin === NULL and $project === NULL)  return NULL;

  // Get the query condition for selecting projects.
  $condition = '';
  $args = array();
  if ($origin != NULL) {
    $condition = 'origin = :origin';
    $args[':origin'] = $origin;
  }
  if ($project != NULL) {
    if ($condition != '')  $condition .= ' AND ';
    $condition .= 'project = :project';
    $args[':project'] = $project;
  }

  // Get a list of projects.
  $get_pguid = 'SELECT pguid FROM {btr_projects} WHERE ' . $condition;
  $arr_pguid = btr_query($get_pguid, $args)->fetchCol();

  return $arr_pguid;
}

/**
 * Delete the template with the given id, as well as the locations
 * and files related to it.
 */
function _erase_template($potid) {
  $args = array(':potid' => $potid);

  // Decrement the count of the strings related to this template.
  $update_strings = '
    UPDATE {btr_strings} AS s
    INNER JOIN (SELECT sguid FROM {btr_locations} WHERE potid = :potid) AS l
            ON (l.sguid = s.sguid)
    SET s.count = s.count - 1';
  btr_query($update_strings, $args);

  // Delete the locations, files, and the template itself.
  btr_delete('btr_locations')->condition('potid', $potid)->execute();
  btr_delete('btr_files')->condition('potid', $potid)->execute();
  btr_delete('btr_templates')->condition('potid', $potid)->execute();
}

