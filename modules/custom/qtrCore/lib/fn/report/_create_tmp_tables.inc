<?php
/**
 * @file
 * Function _create_tmp_tables().
 */

/**
 * Create the temporary tables {qtr_tpm_translations} and {qtr_tmp_votes}
 * with the relevant translations and votes.
 */
function _create_tmp_tables($lng, $origin, $project) {
  // Create a temporary table of the relevant translations.
  $sql_get_translations = "CREATE TEMPORARY TABLE {qtr_tmp_translations} AS
        SELECT t.*
        FROM {qtr_projects} p
        JOIN {qtr_templates} tpl ON (tpl.pguid = p.pguid)
        JOIN {qtr_locations} l ON (l.potid = tpl.potid)
        JOIN {qtr_strings} s ON (l.sguid = s.sguid)
        JOIN {qtr_translations} t ON (t.sguid = s.sguid AND t.lng = :lng)
        WHERE p.origin = :origin";
  $args = [':lng' => $lng, 'origin' => $origin];
  if ($project != NULL) {
    $sql_get_translations .= " AND p.project = :project";
    $args[':project'] = $project;
  }
  qtr::db_query($sql_get_translations, $args);

  // Create a temporary table of the relevant votes.
  $sql_get_votes = "CREATE TEMPORARY TABLE {qtr_tmp_votes} AS
        SELECT v.*
        FROM {qtr_tmp_translations} t
        JOIN {qtr_votes} v ON (v.tguid = t.tguid)
    ";
  qtr::db_query($sql_get_votes);
}
