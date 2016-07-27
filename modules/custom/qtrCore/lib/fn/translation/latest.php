<?php
/**
 * @file
 * Function: translation_latest()
 */

namespace QTranslate;
use \qtr;

/**
 * Get and return an array of the latest translation suggestions,
 * submitted between the last midnight and the midnight before.
 * The fields that are returned are:
 *   tname, cid, vid, string, lng, translation, tguid, time, name, umail
 */
function translation_latest($lng) {

  $get_latest_translations = "
    SELECT v.cid AS chapter_id, v.nr AS verse_nr, v.vid,
           t.lng, t.translation, t.tguid, t.time,
           u.name AS username, u.umail AS usermail
    FROM {qtr_translations} t
    LEFT JOIN {qtr_verses} v ON (v.vid = t.vid)
    LEFT JOIN {qtr_users} u ON (u.umail = t.umail AND u.ulng = t.ulng)
    LEFT JOIN {qtr_chapters} c ON (c.cid = v.cid)
    WHERE t.umail != '' AND t.lng = :lng AND t.time > :from_date
    ORDER BY t.time DESC
    ";

  $args = array(
    ':lng' => $lng,
    ':from_date' => date('Y-m-d', strtotime("-1 day")),
  );

  // run the query and get the translations
  $translations = qtr::db_query($get_latest_translations, $args)->fetchAll();

  return $translations;
}
