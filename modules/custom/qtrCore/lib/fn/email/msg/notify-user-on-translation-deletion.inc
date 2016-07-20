<?php
namespace QTranslate;
use \qtr;

/**
 * Notify the users of a translation, when it is deleted.
 */
{
  $subject = "[$subject_prefix] "
    . t('Deleted: !translation',
      ['!translation' => cut($params->translation, 30)]);

  $body = array();
  $body[] = t('Greetings !username,', ['!username' => $params->username]);
  $body[] = t("A translation that you have supported, has been deleted by a moderator.
You can see the verse and the rest of translations at this link:");
  $body[] = $url;
  $body[] = t('Thanks');

  $verse_header = t('Verse:');
  $deleted_translation = t('Deleted Translation:');
  $body[] = "
    <hr/>

    <h4>$verse_header</h4>
    <ul>
      <li>$params->verse</li>
    </ul>

    <h4>$deleted_translation</h4>
    <ul>
      <li>$params->translation</li>
    </ul>
  ";
}
