<?php
/**
 * @file
 * Function user_create()
 */

namespace QTranslate;

/**
 * Create a new user with the given username and translation_lng.
 * Return the uid of this user.
 */
function user_create($username, $lng = 'en') {
  $site_mail = variable_get('site_mail');
  $user_mail = preg_replace('/\+.*@/', '@', $site_mail);
  $user_mail = str_replace('@', "+${username}@", $user_mail);
  $new_user = array(
    'name' => $username,
    'mail' => $user_mail,
    'init' => $user_mail,
    'translation_lng' => $lng,
  );
  $user = user_save(null, $new_user);
  return $user->uid;
}
