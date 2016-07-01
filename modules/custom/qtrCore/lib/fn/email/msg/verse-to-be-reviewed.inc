<?php
namespace QTranslate;
use \qtr;

/**
 * Send by email a verse to be reviewed.
 */
{
  list($verse, $translations) = qtr::verse_get_translations($params->vid, $lng);
  // Get the url of the verse.
  $url = $client_url . "/qtr/$lng/" . $params->vid;

  $subject = "[$subject_prefix] " . cut($verse, 60);

  $body = array();
  $body[] = t('Greetings !username,', ['!username' => $params->username]);
  $body[] = t('If you have time, please help to review the translation of the verse at this link:');
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
