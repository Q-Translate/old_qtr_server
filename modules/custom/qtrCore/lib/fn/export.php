<?php
/**
 * @file
 * Export translations to file.
 */

namespace QTranslate;
use \qtr;

/**
 * Export the translations of lng/chapter to a file.
 *
 * @param $lng
 *   The language of translations.
 *
 * @param $chapter
 *   The chapter to be exported. If no chapter is given then the whole book will
 *   be exported.
 *
 * @param $mode
 *   Can be 'most_liked' (default), or 'preference'.
 *
 * @param $users
 *   Array of email addresses of users whose translations take precedence.
 *
 * @return
 *   The name of the output file.
 *
 * The export mode 'most_liked' (which is the default one) exports the most
 * liked translations.
 *
 * The export mode 'preference' gives priority to translations that are liked by
 * a certain user or a group of users. It requires an additional argument
 * (users) to specify the user (or a list of users) whose translations are
 * preferred. If there is no translation that is liked by any of the preferred
 * users, then the most liked translation is exported.
 */
function export($lng, $chapter = NULL, $mode = 'most_liked', $users = NULL)
{
  // Check arguments $mode and $users.
  if ($mode != 'most_liked' && $mode != 'preference')  $mode = 'most_liked';
  if ($mode == 'preference' && empty($users)) {
    $account = user_load();
    $users = [$account->init];
  }

  // Get verses that will be exported.
  $select_chapter = ($chapter == NULL ? "1=1" : "v.cid = $chapter");
  $get_verses = "SELECT v.vid, v.verse FROM {qtr_verses} AS v WHERE $select_chapter";
  $verses = qtr::db_query($get_verses)->fetchAllKeyed();

  // Get translations.
  switch ($mode) {
    case 'most_liked':
      $most_liked = _get_most_liked_translations($lng, $select_chapter);
      break;

    case 'preference':
      $preferred = _get_preferred_translations($lng, $select_chapter, $users);
      $most_liked = _get_most_liked_translations($lng, $select_chapter);
      break;
  }

  // Get the translation for each verse.
  $translations = array();
  foreach (array_keys($verses) as $vid) {
    switch ($mode) {
      case 'most_liked':
        if (isset($most_liked[$vid])) {
          $translations[$vid] = $most_liked[$vid];
        }
        else {
          $translations[$vid] = $verses[$vid];
        }
        break;

      case 'preference':
        if (isset($preferred[$vid])) {
          $translations[$vid] = $preferred[$vid];
        }
        else if (isset($most_liked[$vid])) {
          $translations[$vid] = $most_liked[$vid];
        }
        else {
          $translations[$vid] = $verses[$vid];
        }
        break;
    }
  }

  // Write translations to a temporary file.
  $file = tempnam('/tmp', 'export_');
  $fp = fopen($file, 'w');
  foreach ($translations as $vid => $translation)
  fwrite($fp, $translation . "\n");
  fclose($fp);

  // Return the file where translations are exported.
  return $file;
}

/**
 * Get and return an associative array of the most liked translations,
 * indexed by vid. Translations which have no likes at all are skipped.
 */
function _get_most_liked_translations($lng, $select_chapter) {
  // Create a temporary table with the maximum count for each string.
  $max_count = "
      SELECT t.vid, MAX(t.count) AS max_count
      FROM {qtr_verses} AS v
      LEFT JOIN {qtr_translations} AS t
                ON (t.vid = v.vid AND t.lng = :lng)
      WHERE $select_chapter
      GROUP BY t.vid";
  $args = array(':lng' => $lng);
  $tmp_table_max_count = qtr::db_query_temporary($max_count, $args);

  // Get the translations with the max count for each verse.
  // The result will be an assoc array (vid => translation).
  $most_liked_translations = "
      SELECT t.vid, t.translation
      FROM {$tmp_table_max_count} AS cnt
      LEFT JOIN {qtr_translations} AS t
            ON ( t.vid = cnt.vid
            AND  t.count = cnt.max_count
            AND  t.lng = :lng )
      GROUP BY t.vid";
  $result = qtr::db_query($most_liked_translations, $args);
  return $result->fetchAllKeyed();
}

/**
 * Get and return an associative array of the translations that are liked
 * by any of the people in the given array of users, indexed by vid.
 * Translations which have no likes from these users are skipped.
 * Users are identified by their emails.
 */
function _get_preferred_translations($lng, $select_chapter, $users) {
  if (empty($users)) return [];

  // Build a temporary table with translations
  // that have any likes from the given users.
  $translations = "
      SELECT t.vid, t.tguid, t.translation, COUNT(*) AS like_count
      FROM {qtr_verses} AS v
      LEFT JOIN {qtr_translations} AS t
                ON (t.vid = v.vid AND t.lng = :lng)
      LEFT JOIN {qtr_likes} AS l
                ON (l.tguid = t.tguid)
      WHERE $select_chapter
        AND l.umail IN (:users)
      GROUP BY t.tguid
      HAVING COUNT(*) > 0";
  $args = array(
    ':lng' => $lng,
    ':users' => $users,
  );
  $tmp_table_translations = qtr::db_query_temporary($translations, $args);

  // Build a tmp table with the maximum likes for each verse.
  $tmp_table_max_count =
    qtr::db_query_temporary(
      "SELECT vid, MAX(like_count) AS max_count
       FROM {$tmp_table_translations}
       GROUP BY sguid"
    );

  // Get translations with the max count for each verse,
  // as an assoc array (vid => translation).
  $translations = "
      SELECT cnt.vid, t.translation
      FROM {$tmp_table_max_count} AS cnt
      LEFT JOIN {$tmp_table_translations} AS t
                ON (t.vid = cnt.vid AND t.like_count = cnt.max_count)
      GROUP BY cnt.vid";
  return qtr::db_query($translations)->fetchAllKeyed();
}
