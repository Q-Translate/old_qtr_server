<?php
namespace QTranslate;
use \qtr;

/**
 * Notify the author of a translation, when it is deleted.
 */
{
  $subject = "[$subject_prefix] "
    . t('Translation deleted: !translation',
      ['!translation' => cut($params->translation, 30)]);

  $body = array();
  $body[] = t('Greetings !username,', ['!username' => $params->username]);
  $body[] = t("A translation that you have submitted, has been deleted by a moderator.
You can see the verse and the rest of translations at this link:");
  $body[] = $url;
  $body[] = t('Thanks');

  $verse = t('Verse:');
  $deleted_translation = t('Deleted Translation:');
  $body[] = "
    <hr/>

    <h4>$verse</h4>
    <ul>
      <li>$params->verse</li>
    </ul>

    <h4>$deleted_translation</h4>
    <ul>
      <li>$params->translation</li>
    </ul>
  ";
}
