<?php
namespace QTranslate;
use \qtr;

/**
 * Send by email a verse to be reviewed.
 */
{
  list($verse, $translations) = qtr::verse_get_translations($params->vid, $lng);
  // Get the url of the string.
  $url = $client_url . "/qtr/$lng/" . $params->vid;

  $subject = "[$subject_prefix] " . cut($string, 60);

  $body = array();
  $body[] = t('Greetings !username,', ['!username' => $params->username]);
  $body[] = t('If you have time, please help to review the translation of the string at this link:');
  $body[] = $url;
  $body[] = t('Thanks');

  $string_header = t('String:');
  $translations_header = t('Translations:');
  $body[] = "
    <hr/>

    <h4>$string_header</h4>
    <ul>
      <li>$string</li>
    </ul>

    <h4>$translations_header</h4>
    <ul>
  ";
  foreach ($translations as $translation) {
    $body[] = "<li>" . $translation . "</li>\n";
  }
  $body[] = "</ul>\n";
}
