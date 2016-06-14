<?php
/**
 * @file
 * Function: cron_update_mysql_materialized_views()
 */

namespace BTranslator;
use \qtr;

/**
 * Update MySQL materialized views.
 */
function cron_update_mysql_materialized_views() {
  // Materialized views are used to speed-up
  // term autocompletion of vocabularies.
  // For each vocabulary project update the mv table.
  $project_list = qtr::project_ls('vocabulary');
  foreach ($project_list as $project) {
    $project = str_replace('vocabulary/', '', $project);
    $table = 'qtr_mv_' . strtolower($project);
    qtr::db_query("TRUNCATE {$table}");
    qtr::db_query("INSERT INTO {$table}
               SELECT DISTINCT s.string FROM {qtr_strings} s
               JOIN {qtr_locations} l ON (l.sguid = s.sguid)
               JOIN {qtr_templates} t ON (t.potid = l.potid)
               JOIN {qtr_projects}  p ON (p.pguid = t.pguid)
               WHERE p.project = :project
                 AND p.origin = 'vocabulary'
               ORDER BY s.string",
      array(':project' => $project)
    );
  }
}
