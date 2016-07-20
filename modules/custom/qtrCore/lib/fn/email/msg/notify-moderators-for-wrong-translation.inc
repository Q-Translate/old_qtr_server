<?php
namespace QTranslate;
use \qtr;

/**
 *  Notify the moderators of a language about a wrong translation.
 */
{
  $user = user_load($params->author_uid);
  $user_name = $user->name;
  $user_url = $_GLOBALS['base_url'] . '/user/' . $params->author_uid;

  $subject = "[$subject_prefix] "
    . t('Delete: !translation', ['!translation' => cut($params->translation, 30)]);

  $body = array();
  $body[] = t('Greetings !username,', ['!username' => $params->username]);
  $body[] = t("The user !user has tried to delete the translation below, but is not allowed.
If you think he is right, you can delete it here: !url",
            ['!user' => "<a href='$user_url'>$user_name</a>", '!url' => $url]);
  $body[] = t('Thanks');

  $verse_header = t('Verse:');
  $translation = t('Translation:');
  $body[] = "
    <hr/>

    <h4>$verse_header</h4>
    <ul>
      <li>$params->verse</li>
    </ul>

    <h4>$translation</h4>
    <ul>
      <li>$params->translation</li>
    </ul>
  ";
}
