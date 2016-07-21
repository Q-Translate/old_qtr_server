<?php
namespace QTranslate;
use \qtr;

/**
 * Notify the previous users of a verse that a new translation has been
 * submitted. Maybe they would like to review it and change their like.
 */
{
  $subject = "[$subject_prefix] " . t('New translation: !translation',
             ['!translation' => cut($params->new_translation, 30)]);

  $body = array();
  $body[] = t('Greetings !username,', ['!username' => $params->username]);

  $body[] = t("A new translation has been suggested for a verse
that you have supported. If you think that the new
translation is better than the one that you have
supported, then you can change your support at this link:");
  $body[] = $url;
  $body[] = t('Thanks');

  $verse = t('Verse:');
  $liked_translation = t('Supported Translation:');
  $new_translation = t('New Translation:');
  $body[] = "
    <hr/>

    <h4>$verse</h4>
    <ul>
      <li>$params->verse</li>
    </ul>

    <h4>$liked_translation</h4>
    <ul>
      <li>$params->liked_translation</li>
    </ul>

    <h4>$new_translation</h4>
    <ul>
      <li>$params->new_translation</li>
    </ul>
  ";
}
