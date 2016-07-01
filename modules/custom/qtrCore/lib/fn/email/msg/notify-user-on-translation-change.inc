<?php
namespace QTranslate;
use \qtr;

/**
 * Send an email notification to a user, when a translation that he has liked
 * for is modified.
 */
{
  $subject = "[$subject_prefix] "
    . t("Translation modified for: !verse",
      ['!verse' => cut($params->verse, 30)]);

  $body = array();
  $body[] = t('Greetings !username,', ['!username' => $params->username]);
  $body[] = t("The following translation has been modified by its author.
You are being notified because you have liked for the old
translation and your like now counts for the new version
of the translation.

If the new version of the translation is OK for you,
then there is nothing you should do. Otherwise, you
can change your like at this link:");
  $body[] = $url;
  $body[] = t('Thanks');

  $verse = t('Verse:');
  $old_translation = t('Old Translation:');
  $new_translation = t('New Translation:');
  $body[] = "
    <hr/>

    <h4>$verse:</h4>
    <ul>
      <li>$params->verse</li>
    </ul>

    <h4>$old_translation</h4>
    <ul>
      <li>$params->old_translation</li>
    </ul>

    <h4>$new_translation</h4>
    <ul>
      <li>$params->new_translation</li>
    </ul>
   ";
}
