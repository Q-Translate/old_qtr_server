<?php
namespace QTranslate;
use \qtr;

/**
 * Notify the previous users of a verse that a new translation has been
 * submitted. Maybe they would like to review it and change their like.
 */
{
  $subject = "[$subject_prefix]" . t('New translation for: !verse',
             ['!verse' => cut($params->verse, 30)]);

  $body = array();
  $body[] = t('Greetings !username,', ['!username' => $params->username]);

  $body[] = t("A new translation has been suggested for a verse
that you have liked. If you think that the new
translation is more suitable than the one that you have
liked for, then you can change your like at this link:");
  $body[] = $url;
  $body[] = t('Thanks');

  $verse = t('Verse:');
  $liked_translation = t('Liked Translation:');
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
