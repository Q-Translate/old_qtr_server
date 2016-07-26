<?php
namespace QTranslate;
use \qtr;

/**
 * Send by email a verse to be reviewed.
 */
{
  // Get verse and translations.
  $verse = qtr::db_query(
    'SELECT verse FROM {qtr_verses} WHERE vid = :vid',
    [':vid' => $params->vid]
  )->fetchField();
  $translations = qtr::db_query(
    'SELECT translation FROM {qtr_translations} WHERE vid = :vid AND lng = :lng',
    [':vid' => $params->vid, ':lng' => $lng ]
  )->fetchCol();

  $subject = "[$subject_prefix] " . t('Verse: !chapter_id:!verse_nr',
             ['!chapter_id' => $params->chapter_id, '!verse_nr' => $params->verse_nr]);

  $body = array();
  $body[] = t('Greetings !username,', ['!username' => $params->username]);
  $body[] = t('If you have time, please help to review the translation of the verse !chapter_id:!verse_nr',
            ['!chapter_id' => $params->chapter_id, '!verse_nr' => $params->verse_nr]);
  $body[] = $url;
  $body[] = t('Thanks');

  $verse_header = t('Verse:');
  $translations_header = t('Translations:');

  $body[] = "
    <hr/>

    <h4>$verse_header</h4>
    <ul>
      <li>$verse</li>
    </ul>

    <h4>$translations_header</h4>
    <ul>
  ";
  foreach ($translations as $translation) {
    $body[] = "<li>" . $translation . "</li>\n";
  }
  $body[] = "</ul>\n";
}
