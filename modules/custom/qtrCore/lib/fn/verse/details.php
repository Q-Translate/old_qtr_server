<?php
/**
 * @file
 * Function: verse_details()
 */

namespace QTranslate;
use \qtr;

/**
 * Get the details of verses, translations and likes.
 *
 * @param $filter_query
 *   A db_select query object that returns the verses that should be
 *   extracted.
 *
 * @param $lng
 *   Language code of the translations.
 *
 * @param $alternative_langs
 *   Array of alternative (auxiliary) language codes. These will
 *   return translations of other languages, in case that the
 *   translator is not quite familiar with English or needs some help
 *   from another language.
 *
 * @return
 *   An array of verses, translations and likes, where each verse
 *   is an associative array, with translations and likes as nested
 *   associative arrays.
 */
function verse_details($filter_query, $lng, $alternative_langs = array()) {
  // Get the IDs of the verses that are returned by the filter query.
  $assoc_arr_vid = $filter_query->execute()->fetchAllAssoc('vid');
  if (empty($assoc_arr_vid))  return array();
  $arr_vid = array_keys($assoc_arr_vid);

  // Get verses.
  $arr_verses = qtr::db_query(
    'SELECT vid, cid, nr, verse FROM {qtr_verses} WHERE vid IN (:arr_vid)',
    array(':arr_vid' => $arr_vid)
  )->fetchAllAssoc('vid');

  // Get translations.
  $arr_translations = qtr::db_query(
    'SELECT v.vid, t.tguid, t.lng, t.translation,
            t.time, u.name AS author, u.umail, u.ulng, u.uid, t.count
     FROM {qtr_verses} v
     JOIN {qtr_translations} t ON (v.vid = t.vid)
     LEFT JOIN {qtr_users} u ON (u.umail = t.umail AND u.ulng = t.ulng)
     WHERE (t.lng = :lng) AND v.vid IN (:arr_vid)
     ORDER BY t.count DESC',
    array(':lng' => $lng, ':arr_vid' => $arr_vid)
  )->fetchAllAssoc('tguid');

  // Get likes.
  $arr_tguid = array_keys($arr_translations);
  if (empty($arr_tguid)) {
    $arr_likes = array();
  }
  else {
    $arr_likes = qtr::db_query(
      'SELECT t.tguid, l.lid, u.name, u.umail, u.ulng, u.uid, l.time
       FROM {qtr_translations} t
       JOIN {qtr_likes} l ON (l.tguid = t.tguid)
       JOIN {qtr_users} u ON (u.umail = l.umail AND l.ulng = l.ulng)
       WHERE t.tguid IN (:arr_tguid)
       ORDER BY l.time DESC',
      array(':arr_tguid' => $arr_tguid)
    )->fetchAllAssoc('vid');
  }

  // Get alternatives (from other languages). They are the best
  // translations (max count) from the alternative languages.
  if (empty($alternative_langs)) {
    $arr_alternatives = array();
  }
  else {
    $arr_alternatives = qtr::db_query(
      'SELECT DISTINCT t.vid, t.tguid, t.lng, t.translation, t.count
       FROM (SELECT vid, lng, MAX(count) AS max_count
	     FROM {qtr_translations}
	     WHERE lng IN (:arr_lng) AND vid IN (:arr_vid)
	     GROUP BY vid, lng
	     ) AS m
       INNER JOIN {qtr_translations} AS t
	     ON (m.vid = t.vid AND m.lng = t.lng AND m.max_count = t.count)
       GROUP BY t.vid, t.lng, t.count',
      array(
        ':arr_lng' => $alternative_langs,
        ':arr_vid' => $arr_vid,
      )
    )->fetchAllAssoc('tguid');
  }

  // Put alternatives as nested array under verses.
  foreach ($arr_alternatives as $tguid => $alternative) {
    $vid = $alternative->vid;
    $lng = $alternative->lng;
    $arr_verses[$vid]->alternatives[$lng] = $alternative->translation;
  }

  // Put likes as nested arrays inside translations.
  // Likes are already ordered by time (desc).
  foreach ($arr_likes as $vid => $like) {
    $tguid = $like->tguid;
    $name = $like->name;
    $arr_translations[$tguid]->likes[$name] = $like;
  }

  // Put translations as nested arrays inside verses.
  // Translations are already ordered by count (desc).
  // Make sure that each translation has an array of likes
  // (even though it may be empty).
  foreach ($arr_translations as $tguid => $translation) {
    if (!isset($translation->likes))  $translation->likes = array();
    $vid = $translation->vid;
    $arr_verses[$vid]->translations[$tguid] = $translation;
  }

  // Put verses in the same order as $arr_vid.
  // Make sure as well that each verse has an array of translations
  // (even though it may be empty).
  $verses = array();
  foreach ($arr_vid as $vid) {
    $verse = $arr_verses[$vid];
    if (!isset($verse->alternatives)) $verse->alternatives = array();
    if (!isset($verse->translations)) $verse->translations = array();
    $verse->translations = array_values($verse->translations);
    $verses[] = $verse;
  }

  return $verses;
}
