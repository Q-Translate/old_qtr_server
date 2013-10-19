<?php
/**
 * @file
 * Functions specific to the menu path: translations/vote .
 */

/**
 * Include utility and theme functions.
 */
require_once(dirname(__FILE__) . '/list/list.inc');
require_once(dirname(__FILE__) . '/utils.inc');

/**
 * Menu callback: translations/vote
 * Vote the best translation for a string, or suggest a better translation.
 */
function btr_ui_vote($lng =NULL, $sguid =NULL, $one_time_login =NULL) {

  if ($one_time_login != NULL) {
    btr_ui_login_one_time($one_time_login);
    drupal_goto("translations/$lng/$sguid");
    return;
  }

  if ($lng == NULL) {
    $lng = variable_get('btr_translation_lng', 'fr');
  }

  if ($sguid == NULL or $sguid == 'untranslated' or $sguid == 'translated') {
    if ($sguid == 'untranslated') {
      $sguid = btr_ui_get_untranslated_sguid();
    }
    elseif ($sguid == 'translated') {
      $sguid = btr_ui_get_translated_sguid();
    }
    else {
      $sguid = btr_ui_get_sguid();
    }
    if ($sguid=='') {
      drupal_set_message(t('No string found! Please check preferred projects and other settings!'), 'warning');
      drupal_goto("/");
      return;
    }
    $_SESSION['sguid'] = $sguid;
    drupal_goto("translations/$lng/$sguid");
    return;
  }

  // Add the CSS and JS files.
  drupal_add_css(drupal_get_path('module', 'btr_ui') . '/editor/editor.css');
  drupal_add_js(drupal_get_path('module', 'btr_ui') . '/editor/jquery.worddiff.js');
  drupal_add_js(drupal_get_path('module', 'btr_ui') . '/editor/editor.js');

  // Get languages and language codes
  $languages = btr_get_languages();
  $lng_codes = array_keys($languages);

  // Add RTL style if the current language's direction is RTL
  if ($languages[$lng]['direction'] == LANGUAGE_RTL) {
    drupal_add_css(drupal_get_path('module', 'btr_ui') . '/editor-rtl.css');
  }

  /*
  // Add missing breadcrumb.
  drupal_set_breadcrumb(
    array(
      l(t('Home'), NULL),
      l(t('Translations'), 'translations'),
    )
  );
  */

  // Set the title.
  drupal_set_title(t('Vote Translations'));

  // Get the string and its translations.
  global $user;
  $other_langs = btr_ui_get_auxiliary_languages(user_load($user->uid));
  $strings = btr_get_strings(array($sguid), $lng, $other_langs);
  //if ($strings) dpm($strings);  //debug

  // Output the string and its translations.
  $output = drupal_get_form('btr_ui_translate_form', $strings, $lng);

  return $output;
}

function btr_ui_login_one_time($one_time_login =NULL) {

  if ($one_time_login == NULL)  return FALSE;

  // Check if the user is already logged in.
  global $user;
  if ($user->uid)  return FALSE;

  // Get $timestamp, $uid, and $hashed_pass
  $parts = preg_split('/\./', $one_time_login, 3);
  $timestamp = isset($parts[0]) ? $parts[0] : '';
  $uid = isset($parts[1]) ? $parts[1] : '';
  $hashed_pass = isset($parts[2]) ? $parts[2] : '';
  if ($timestamp == '' or $uid == '' or $hashed_pass == '')  return FALSE;

  // Get the user details.
  $account = user_load($uid);
  if (!isset($account) || $account->status == 0)  return FALSE;

  // Check the timestamp.
  $timeout = 86400;  // 24 hours
  $current = REQUEST_TIME;
  if ($current - $timestamp > $timeout) {
    drupal_set_message(t('Login link has expired.'));
    return FALSE;
  }

  // Check the hashed password.
  if ($hashed_pass != user_pass_rehash($account->pass, $timestamp, $account->login)) {
    drupal_set_message(t('Could not validate login link.'));
    return FALSE;
  }

  // Login the new user.
  $user = $account;
  user_login_finalize();
  return TRUE;
}


/**
 * Return a sguid from the strings that should be voted.
 * This string is selected according to the preferencies
 * of the given user. If no uid is given as parameter,
 * then the current user is assumed.
 */
function btr_ui_get_sguid($uid =NULL) {

  // if no parameter is given, get the uid of the current user
  if ($uid == NULL) {
    global $user;
    $uid = $user->uid;
  }

  // get the string-order preference for the user
  $string_order = btr_ui_get_string_order(user_load($uid));

  // select the string according to the string-order preference
  switch ($string_order) {
    default:
    case 'random':
      $sguid = btr_ui_get_random_sguid($uid);
      break;

    case 'sequential':
      $sguid = btr_ui_get_sequential_sguid($uid);
      break;
  }

  return $sguid;
}


/**
 * Return a random sguid from the preferred projects of the user.
 */
function btr_ui_get_random_sguid($uid) {
  // get the preferred projects (as an array)
  $preferred_projects = btr_ui_get_preferred_projects(user_load($uid));
  return btr_get_random_sguid($preferred_projects);
}

/**
 * Return a random untranslated sguid from the preferred projects of the user.
 */
function btr_ui_get_untranslated_sguid() {
  $account = user_load($GLOBALS['user']->uid);
  $preferred_projects = btr_ui_get_preferred_projects($account);
  $lng = variable_get('btr_translation_lng', 'fr');
  return btr_get_untranslated_sguid($preferred_projects, $lng);
}

/**
 * Return a random translated sguid from the preferred projects of the user.
 */
function btr_ui_get_translated_sguid() {
  $account = user_load($GLOBALS['user']->uid);
  $preferred_projects = btr_ui_get_preferred_projects($account);
  $lng = variable_get('btr_translation_lng', 'fr');
  return btr_get_translated_sguid($preferred_projects, $lng);
}


/**
 * Return a sequential sguid from the preferred projects of the user.
 */
function btr_ui_get_sequential_sguid($uid) {

  // get the sequential data
  $account = user_load($uid);
  $sequential = btr_ui_get_data_sequential($account);

  // if sequential data are not set, then just return a random sguid
  if ($sequential == NULL) {
    drupal_set_message(t(
        "It seems that there is something wrong with your preferences. Please <a href='@edit-profile'>check your profile</a>.",
        array('@edit-profile' => "/user/$uid/edit-profile")),
      'warning');
    return btr_ui_get_random_sguid($uid);
  }

  $lid = $sequential->lid;
  $idx = $sequential->idx;
  $proj = $sequential->projects[$idx];

  // get the id of the next string location
  $lid++;
  if ($lid > $proj->lmax) {
    $idx++;
    if ($idx >= sizeof($sequential->projects)) {
      drupal_set_message(t(
          "You have reached the end of the preferred project. Please <a href='@edit-profile'>visit your profile</a> and modify them.",
        array('@edit-profile' => '/user/$uid/edit-profile')),
      'status');
      return btr_ui_get_random_sguid($uid);
    }
    $proj = $sequential->projects[$idx];
    $lid = $proj->lmin;
  }

  // save the new index of the object $sequential
  $sequential->lid = $lid;
  $sequential->idx = $idx;
  //dpm($sequential);  //debug
  $edit['field_data_sequential']['und'][0]['value'] = serialize($sequential);
  $edit['skip_presave'] = TRUE;
  user_save($account, $edit);

  // get and return the sguid
  return btr_get_sguid_from_lid($lid);
}