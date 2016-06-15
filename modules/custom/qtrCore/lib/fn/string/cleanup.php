<?php
/**
 * @file
 * Definition of function string_cleanup().
 */

namespace QTranslate;
use \qtr;

/**
 * Delete any dangling strings that don't belong to any project, and their
 * translations that have no votes.
 *
 * @param $purge
 *   If true, delete as well translations that have votes.
 */
function string_cleanup($purge = FALSE) {
  // Create a temporary table with the dangling strings.
  $dangling_strings =
    qtr::db_query_temporary(
      'SELECT s.sguid
       FROM {qtr_strings} s
       LEFT JOIN {qtr_locations} l ON (l.sguid = s.sguid)
       WHERE l.lid IS NULL'
    );

  // Count the dangling strings.
  $count = qtr::db_query(
    "SELECT COUNT(*) AS cnt FROM {$dangling_strings}"
  )->fetchField();

  // If there are no dangling strings, stop here.
  if (!$count)  return;

  // Delete translations of dangling strings that have no votes.
  qtr::db_query(
    "DELETE t.* FROM {qtr_translations} t
     INNER JOIN {$dangling_strings} d ON (d.sguid = t.sguid)
     LEFT JOIN {qtr_votes} v ON (v.tguid = t.tguid)
     WHERE v.vid IS NULL"
  );

  // Delete translations that have votes as well.
  if ($purge) {
    // Get a list of translations that will be deleted.
    $tguid_list = qtr::db_query(
      "SELECT t.tguid FROM {qtr_translations} t
       INNER JOIN {$dangling_strings} d ON (d.sguid = t.sguid)"
    )->fetchCol();

    // Delete each translation (and related votes as well).
    foreach ($tguid_list as $tguid) {
      qtr::translation_del($tguid, $notify=FALSE, $uid=1);
    }
  }

  // Delete the dangling strings itself.
  qtr::db_query(
    "DELETE s.* FROM {qtr_strings} s
     INNER JOIN {$dangling_strings} d ON (d.sguid = s.sguid)"
  );
}
