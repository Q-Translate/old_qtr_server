<?php
/**
 * @file
 * Get a translated random sguid.
 */

namespace BTranslator;
use \qtr;

include_once(dirname(__FILE__). '/get_pool_of_projects.inc');

/**
 * Get an untranslated sguid from the projects of the given user.
 *
 * @param $lng
 *   Language of translations.
 *
 * @param $uid
 *   Select according to the preferencies of this user.
 *   If no $uid is given, then the current user is assumed.
 *
 * @param $scope
 *   Array of projects to restrict selection.
 *
 * @return
 *   The sguid of a randomly selected untranslated string.
 */
function sguid_get_untranslated($lng, $uid =NULL, $scope =NULL) {
  // Get the list of projects that will be searched.
  $projects = get_pool_of_projects($uid, $scope);

  // Build the WHERE condition for selecting projects.
  list($where, $args) = qtr::utils_projects_to_where_condition($projects);
  $args[':lng'] = $lng;
  if ($where == '')  $where = '(1=1)';

  // Get the total number of strings from which we can choose.
  $sql_count = "
    SELECT COUNT(*) AS number_of_strings
    FROM (SELECT pguid FROM {qtr_projects} WHERE $where) p
    LEFT JOIN {qtr_templates} tpl ON (tpl.pguid = p.pguid)
    LEFT JOIN {qtr_locations} l ON (l.potid = tpl.potid)
    LEFT JOIN {qtr_strings} s ON (s.sguid = l.sguid)
    LEFT JOIN {qtr_translations} t ON (t.sguid = s.sguid AND t.lng = :lng)
    WHERE t.sguid IS NULL
  ";
  $nr_strings = qtr::db_query($sql_count, $args)->fetchField();

  // Get the sguid of a random untranslated. 'count' keeps in how many projects
  // a string appears. Ordering by 'count' and getting a random string
  // from the top, we try to get an untranslated string that appears
  // in other projects as well.
  $random_row_number = rand(0, ceil($nr_strings/3));
  $sql_get_sguid = "
    SELECT s.sguid
    FROM (SELECT pguid FROM {qtr_projects} WHERE $where) p
    LEFT JOIN {qtr_templates} tpl ON (tpl.pguid = p.pguid)
    LEFT JOIN {qtr_locations} l ON (l.potid = tpl.potid)
    LEFT JOIN {qtr_strings} s ON (s.sguid = l.sguid)
    LEFT JOIN {qtr_translations} t ON (t.sguid = s.sguid AND t.lng = :lng)
    WHERE t.sguid IS NULL
    ORDER BY s.count DESC
    LIMIT $random_row_number, 1
  ";
  $sguid = qtr::db_query($sql_get_sguid, $args)->fetchField();

  return $sguid;
}
