<?php
/**
 * @file
 * Creating a vocabulary.
 */

namespace QTranslate;
use \qtr;

/**
 * Create a new vocabulary.
 *
 * @param $name
 *   The name of the vocabulary.
 *
 * @param $lng
 *   The language of the vocabulary.
 *
 * @param $uid
 *   ID of the user that is creating the vocabulary.
 */
function vocabulary_add($name, $lng, $uid = NULL) {
  $uid = qtr::user_check($uid);
  $origin = 'vocabulary';
  $project = $name . '_' . $lng;
  $path = drupal_get_path('module', 'qtrCore');
  $pot_file = '/tmp/' . $project . '.pot';
  touch($pot_file);
  qtr::project_add($origin, $project, $pot_file, $uid);
  unlink($pot_file);

  // Create a custom contact form.
  \db_delete('contact')->condition('category', $project)->execute();
  \db_insert('contact')->fields([
      'category' => $project,
      'recipients' => variable_get('site_mail'),
      'reply' => '',
    ])->execute();

  // Add user as admin of the project.
  qtr::project_add_admin($origin, $project, $lng, $uid);

  // Subscribe user to this project.
  qtr::project_subscribe($origin, $project, $uid);

  // Update mv table.
  $table = 'qtr_mv_' . strtolower($project);
  qtr::db_query("DROP TABLE IF EXISTS {$table}");
  qtr::db_query("CREATE TABLE {$table} LIKE {qtr_mv_sample}");
  qtr::db_query("INSERT INTO {$table}
	SELECT DISTINCT s.string FROM {qtr_strings} s
	JOIN {qtr_locations} l ON (l.sguid = s.sguid)
	JOIN {qtr_templates} t ON (t.potid = l.potid)
	JOIN {qtr_projects}  p ON (p.pguid = t.pguid)
	WHERE p.project = :project
	  AND p.origin = 'vocabulary'
	ORDER BY s.string",
    [':project' => $project]
  );
}
