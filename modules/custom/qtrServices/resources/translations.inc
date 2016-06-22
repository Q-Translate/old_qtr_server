<?php
/**
 * @file
 * Resource definition and callbacks for translations.
 */

function _qtrServices_translations() {
  $file = array('type' => 'inc', 'module' => 'qtrServices', 'name' => 'resources/translations');

  // Return a string, along with its translations,
  // likes and other details.
  $translations['operations']['retrieve'] = array(
    'help' => 'Return a string, along with its translations, likes and other details.',
    'file' => $file,
    'callback' => '_qtrServices_translations_retrieve',
    'args' => array(
      array(
        'name' => 'vid',
        'optional' => FALSE,
        'source' => array('path' => 0),
        'description' => 'The string (globally unique) id.',
        'type' => 'string',
      ),
      array(
        'name' => 'lng',
        'optional' => FALSE,
        'source' => array('param' => 'lng'),
        'description' => 'The language of translations.',
        'type' => 'string',
      ),
    ),
    'access callback' => 'user_access',
    'access arguments' => array('access content'),
  );

  // Filter strings and translations
  // according to the given parameters.
  $translations['operations']['index'] = array(
    'help' => 'Index of strings and translations.',
    'file' => $file,
    'callback' => '_qtrServices_translations_index',
    'args' => array(
      array(
        'name' => 'params',
        'optional' => FALSE,
        'source' => 'param',
        'description' => 'Filter parameters (lng, words, project, etc.)',
        'type' => 'array',
      ),
    ),
    'access callback' => 'user_access',
    'access arguments' => array('access content'),
  );

  // Search strings and translations using various filters.
  $translations['actions']['search'] = array(
    'help' => 'Search strings and translations.',
    'file' => $file,
    'callback' => '_qtrServices_translations_search',
    'args' => array(
      array(
        'name' => 'params',
        'optional' => FALSE,
        'source' => 'data',
        'description' => 'Filters (lng, words, project, etc.)',
        'type' => 'array',
      ),
    ),
    'access callback' => 'user_access',
    'access arguments' => array('access content'),
  );

  // Get the vid of a random string according to the target.
  $translations['actions']['get_random_vid'] = array(
    'help' => 'Get the vid of a random string according to the target, which can be: random|translated|untranslated.',
    'file' => $file,
    'callback' => '_qtrServices_translations_get_random_vid',
    'args' => array(
      array(
        'name' => 'params',
        'optional' => FALSE,
        'source' => 'data',
        'description' => 'Assoc array containing the target.',
        'type' => 'array',
      ),
    ),
    'access callback' => 'user_access',
    'access arguments' => array('access content'),
  );

  // Get a string, along with its translations, likes and other details.
  $translations['actions']['get'] = array(
    'help' => 'Return a string, along with its translations, likes and other details.',
    'file' => $file,
    'callback' => '_qtrServices_translations_get',
    'args' => array(
      array(
        'name' => 'params',
        'optional' => FALSE,
        'source' => 'data',
        'description' => 'Parameters (vid and lng).',
        'type' => 'array',
      ),
    ),
    'access callback' => 'user_access',
    'access arguments' => array('access content'),
  );

  // Add a new translation to a source string.
  $translations['actions']['add'] = array(
    'help' => 'Add a new translation to a source string.',
    'file' => $file,
    'callback' => '_qtrServices_translations_add',
    'args' => array(
      array(
        'name' => 'params',
        'optional' => FALSE,
        'source' => 'data',
        'description' => 'Translation data (vid, lng, translation).',
        'type' => 'array',
      ),
    ),
    'access callback' => 'user_access',
    'access arguments' => array('qtranslate-suggest'),
  );

  // Add a like for a translation.
  $translations['actions']['like'] = array(
    'help' => 'Add a like for a translation.',
    'file' => $file,
    'callback' => '_qtrServices_translations_like',
    'args' => array(
      array(
        'name' => 'params',
        'optional' => FALSE,
        'source' => 'data',
        'description' => 'Translation to be liked (tguid).',
        'type' => 'array',
      ),
    ),
    'access callback' => 'user_access',
    'access arguments' => array('qtranslate-like'),
  );

  // Delete a translation.
  $translations['actions']['del'] = array(
    'help' => 'Delete a translation.',
    'file' => $file,
    'callback' => '_qtrServices_translations_del',
    'args' => array(
      array(
        'name' => 'params',
        'optional' => FALSE,
        'source' => 'data',
        'description' => 'Translation (tguid) to be deleted.',
        'type' => 'array',
      ),
    ),
    'access callback' => 'user_access',
    'access arguments' => array('qtranslate-suggest'),
  );

  // Delete a like from a translation.
  $translations['actions']['del_like'] = array(
    'help' => 'Delete a like from a translation.',
    'file' => $file,
    'callback' => '_qtrServices_translations_del_like',
    'args' => array(
      array(
        'name' => 'params',
        'optional' => FALSE,
        'source' => 'data',
        'description' => 'Translation (tguid) to be un-liked.',
        'type' => 'array',
      ),
    ),
    'access callback' => 'user_access',
    'access arguments' => array('qtranslate-like'),
  );

  // Submit multiple actions at once.
  $translations['actions']['submit'] = array(
    'help' => 'Submit multiple actions at once.',
    'file' => $file,
    'callback' => '_qtrServices_translations_submit',
    'args' => array(
      array(
        'name' => 'params',
        'optional' => FALSE,
        'source' => 'data',
        'description' => 'Array of actions to be executed.',
        'type' => 'array',
      ),
    ),
    'access callback' => 'user_access',
    'access arguments' => array('qtranslate-like'),
  );

  // Import translations and likes from PO files.
  $translations['actions']['import'] = array(
    'help' => 'Import translations and likes from PO files.',
    'file' => $file,
    'callback' => '_qtrServices_like_import',
    'args' => array(
      array(
        'name' => 'params',
        'optional' => FALSE,
        'source' => 'data',
        'description' => 'Assoc array of parameters.',
        'type' => 'array',
      ),
    ),
    'access callback' => 'user_access',
    'access arguments' => array('qtranslate-like'),
  );

  return $translations;
}

/**
 * Get a string, along with its translations likes and other details.
 *
 * @param $vid
 *   The string (globally unique) id, calculated
 *   as: $vid = sha1($string . $context)
 *   It can also be: random|translated|untranslated
 *
 * @param $lng
 *   The language of translations, like: 'fr', 'sq', etc.
 *
 * @return
 *   A nested structure containing the string, its
 *   translations, likes, etc.
 *
 * @example
 *   curl -k -i \
 *     'https://dev.qtranslate.org/api/translations/random?lng=sq'
 */
function _qtrServices_translations_retrieve($vid, $lng) {
  $strings = _qtrServices_translations_get(array(
               'vid' => $vid,
               'lng' => $lng,
             ));
  return $strings;
}


/**
 * Filter strings and translations according to the given parameters.
 *
 * This provides the same functionality and has the same parameters
 * as the operation 'translations/search'. The only difference is that
 * the parameters are provided as a GET request (in the URL), whereas
 * the parameters of 'translations/search' are provided as a POST
 * request. Sometimes this may be more convenient.
 *
 * @param $params
 *   Parameters for filtering the strings etc.
 *
 * @return
 *   Array containing search results, along with the filters
 *   and the pager info (same as for the search case).
 *
 * @example
 *   curl -k -i -H 'Accept: application/json' \
 *     'https://dev.qtranslate.org/api/translations?lng=sq&words=file'
 */
function _qtrServices_translations_index($params) {
  return _qtrServices_translations_search($params);
}


/**
 * Search strings and translations using various filters.
 *
 * @param $params
 *   Associative array of the POST data, which contains
 *   the filter parameters. These parameters can be:
 *   - lng
 *       The language of translations.
 *   - limit
 *       The number of results to be returned (min: 5, max: 50).
 *   - words
 *       Words to be searched for.
 *   - vid
 *       If the 'words' is empty and there is a 'vid' then the
 *       words of this string are used for searching.
 *   - mode
 *       Search mode. Can be one of:
 *       - natural-strings       Natural search on strings (default).
 *       - natural-translations  Natural search on translations.
 *       - boolean-strings       Boolean search on strings.
 *       - boolean-translations  Boolean search on translations.
 *   - page
 *       Page of results to be displayed.
 *   - project
 *       Limit search only to this project
 *   - origin
 *       Limit search only to the projects of this origin.
 *   - only_mine (boolean)
 *       Limit search only to the strings touched (translated or liked)
 *       by the current user.
 *   - translated_by
 *       Limit search by the author of translations
 *       (can be used only by admins).
 *   - liked_by
 *       Limit search by a user (can be used only by admins).
 *   - date_filter
 *       Which date to filter (used only by admins). Can be one of:
 *       - strings       Filter Strings By Date
 *       - translations  Filter Translations By Date (default)
 *       - likes         Filter Likes By Date
 *   - from_date
 *   - to_date
 *   - list_mode
 *       Used when listing the strings of a project.
 *       Can be one of: 'all', 'translated', 'untranslated'
 *
 * @return
 *   Array containing search results, along with the filters
 *   and the pager info. Each result is a nested structure
 *   containing the string, its translations, likes, etc.
 */
function _qtrServices_translations_search($params) {
  // Get and check the parameter 'lng'.
  $lng = $params['lng'];
  if (!in_array($lng, qtr::lng_get())) {
    return services_error('Language missing or not supported.', 406);
  }

  // The PagerDefault of Drupal7 queries gets the current page
  // from $_GET['page']. This is a trick to make it work.
  // See: https://api.drupal.org/api/drupal/includes%21pager.inc/function/pager_find_page/7
  $_GET['page'] = isset($params['page']) ? (int)trim($params['page']) : 0;

  // Build the filter and the query, and then get the strings.
  $filter = qtr::search_build_filter($params);
  $query = qtr::search_build_query($filter);
  $strings = ($query===NULL ? array() : qtr::string_details($query, $lng));

  // The PagerDefault of Drupal7 defines the pager variables
  // as global variables (these variables are later used by theme_pager).
  // See: https://api.drupal.org/api/drupal/includes!pager.inc/function/pager_default_initialize/7
  $pager = array(
    'number_of_items' => $GLOBALS['pager_total_items'][0],
    'items_per_page' => $GLOBALS['pager_limits'][0],
    'number_of_pages' => $GLOBALS['pager_total'][0],
    'current_page' => $GLOBALS['pager_page_array'][0],
  );

  return array(
    'filter' => $filter,
    'pager' => $pager,
    'strings' => $strings,
  );
}


/**
 * Get the vid of a random string according to the target.
 *
 * @param $params
 *   Associative array of the POST data, which contains:
 *   - target
 *       Can be one of: random|translated|untranslated
 *   - lng
 *       Code of the language (required for translated and untranslated)
 *   - scope (optional)
 *       If provided, restricts the pool of selection to the given
 *       list of project. It is a space separated list of origin/project
 *       like this: 'origin1/project1 origin2 origin1/project2'
 *
 * @return
 *   Associative array containing:
 *   - vid
 *       ID of a random string, or the target itself
 *       if it is different from random|translated|untranslated
 */
function _qtrServices_translations_get_random_vid($params) {
  $projects = NULL;
  if (isset($params['scope'])) {
    $projects = array();
    $scope = preg_replace('/\s+/', ' ', trim($params['scope']));
    $arr = explode(' ', $scope);
    foreach ($arr as $proj) {
      $projects[] = $proj;
    }
  }
  $target = $params['target'];
  $lng = $params['lng'];
  $vid = qtr::vid_get($target, $lng, $projects);

  return [ 'vid' => $vid ];
}


/**
 * Get a string, along with its translations likes and other details.
 *
 * @param $params
 *   Associative array of the POST data, which contains:
 *   - vid
 *       The string (globally unique) id, calculated
 *       as: $vid = sha1($string . $context)
 *       It can also be: random|translated|untranslated
 *   - lng
 *       The language of translations, like: 'fr', 'sq', etc.
 *
 * @return
 *   Associative array comtaining:
 *    - vid
 *        A nested structure containing the string, its
 *        translations, likes, etc.
 *
 * @example
 *   curl -k -i -H 'Content-type: application/json' \
 *     -X POST https://dev.qtranslate.org/api/translations/get
 *     -d '{"lng": "sq"}'
 */
function _qtrServices_translations_get($params) {
  $lng = $params['lng'];
  if (!in_array($lng, qtr::lng_get())) {
    return services_error('Language missing or not supported.', 406);
  }

  $vid = $params['vid'];
  if (in_array($vid, ['random', 'translated', 'untranslated'])) {
    $vid = qtr::vid_get($vid, $lng);
  }

  $strings = qtr::string_load([$vid], $lng);
  $string = empty($strings) ? NULL : $strings[0];

  return [ 'string' => $string ];
}

/**
 * Add a new translation to a source string.
 *
 * @param $params
 *   Associative array of the POST data, which contains:
 *   - vid
 *       The string ID for which a new translation should be added.
 *   - lng
 *       The language (code) of the new translation.
 *   - translation
 *       The new translation as a string. If the string has plural
 *       version(s) as well, they are concatenated with NULL bytes ("\0")
 *       between them.
 *
 * @return
 *   Associative array containing:
 *   - tguid
 *       ID of the new translation, or NULL if no translation was added.
 *   - messages
 *       Array of notification messages; each notification message
 *       is an array of a message and a type, where type can be one of
 *       'status', 'warning', 'error'.
 */
function _qtrServices_translations_add($params) {
  // get parameters and validate them
  $vid = $params['vid'];
  $translation = $params['translation'];
  $lng = $params['lng'];
  if (!in_array($lng, qtr::lng_get())) {
    return services_error(t('Language missing or not supported.'), 406);
  }

  // Check access permissions.
  if (!user_access('qtranslate-suggest')) {
    $msg = t('No rights for contributing suggestions!');
    return ['tguid' => NULL, 'messages' => [[$msg, 'error']]];
  }
  // Check that the language matches the translation language of the user.
  $account = user_load($GLOBALS['user']->uid);
  if (!user_access('qtranslate-admin') and ($lng != $account->translation_lng)) {
    $msg = t('No rights for contributing to language <strong>!lng</strong>.', ['!lng' => $lng]);
    return ['tguid' => NULL, 'messages' => [[$msg, 'error']]];
  }

  // Add the translation.
  $tguid = qtr::translation_add($vid, $lng, $translation);
  return ['tguid' => $tguid, 'messages' => qtr::messages()];
}


/**
 * Add a like for the given translation.
 *
 * @param $params
 *   Associative array of the POST data, which contains:
 *   - tguid
 *       ID of the translation
 *
 * @return
 *   Associative array containing:
 *   - vid
 *       ID of the new like, or NULL
 *   - messages
 *       Array of notification messages; each notification message
 *       is an array of a message and a type, where type can be one of
 *       'status', 'warning', 'error'.
 */
function _qtrServices_translations_like($params) {
  $tguid = $params['tguid'];

  // Check access permissions.
  if (!user_access('qtranslate-like')) {
    $msg = t('No rights for submitting likes!');
    return ['vid' => NULL, 'messages' => [[$msg, 'error']]];
  }

  // Add the like.
  $vid = qtr::like_add($tguid);
  return ['vid' => $vid, 'messages' => qtr::messages()];
}

/**
 * Delete a translation.
 *
 * @param $params
 *   Associative array of the POST data, which contains:
 *   - tguid
 *       ID of the translation
 *
 * @return
 *   Associative array containing:
 *   - messages
 *       Array of notification messages; each notification message
 *       is an array of a message and a type, where type can be one of
 *       'status', 'warning', 'error'.
 */
function _qtrServices_translations_del($params) {
  $tguid = $params['tguid'];
  qtr::translation_del($tguid);
  return ['messages' => qtr::messages()];
}

/**
 * Delete a like from the given translation.
 *
 * @param $params
 *   Associative array of the POST data, which contains:
 *   - tguid
 *       ID of the translation
 *
 * @return
 *   Associative array containing:
 *   - messages
 *       Array of notification messages; each notification message
 *       is an array of a message and a type, where type can be one of
 *       'status', 'warning', 'error'.
 */
function _qtrServices_translations_del_like($params) {
  $tguid = $params['tguid'];
  qtr::like_del($tguid);
  return ['messages' => qtr::messages()];
}

/**
 * Submit multiple actions at once.
 *
 * @param $params
 *   Array of the POST data, which contains an item for each action.
 *   Each action is an associated array that contains the items:
 *   - action
 *       The action to be done (add, like, del, del_like)
 *   - params
 *       Associative array of the needed parameters for this action.
 *
 * @return
 *   Associative array containing:
 *   - messages
 *       Array of notification messages; each notification message
 *       is an array of a message and a type, where type can be one of
 *       'status', 'warning', 'error'.
 */
function _qtrServices_translations_submit($params) {
  $nr_likes = $nr_suggestions = $nr_deletions = 0;
  foreach ($params as $item) {
    $action = $item['action'];
    if (!$action) continue;
    switch ($action) {
    case 'add':
      $result = _qtrServices_translations_add($item['params']);
      if ($result['tguid'] != NULL)  $nr_suggestions++;
      qtr::messages($result['messages']);
      break;

    case 'like':
      $result = _qtrServices_translations_like($item['params']);
      if ($result['vid'] != NULL)  $nr_likes++;
      qtr::messages($result['messages']);
      break;

    case 'del':
      $result = _qtrServices_translations_del($item['params']);
      if (sizeof($result['messages'])==0) {
        $nr_deletions++;
      }
      else {
        qtr::messages($result['messages']);
      }
      break;

    case 'del_like':
      $result = _qtrServices_translations_del_like($item['params']);
      qtr::messages($result['messages']);
      break;

    default:
      $msg = t('Action !action is not supported!', ['!action' => $action]);
      qtr::messages($msg, 'error');
      break;
    }
  }

  if ($nr_suggestions > 0) {
    $msg = format_plural($nr_suggestions, '1 suggestion saved', '@count suggestions saved');
    qtr::messages($msg);
  }
  if ($nr_likes > 0) {
    $msg = format_plural($nr_likes, '1 like saved', '@count likes saved');
    qtr::messages($msg);
  }
  if ($nr_deletions > 0) {
    $msg = format_plural($nr_deletions, '1 translation deleted', '@count translations deleted');
    qtr::messages($msg);
  }

  return ['messages' => qtr::messages()];
}

/**
 * Bulk import of translations and likes from PO files.
 *
 * @param $params
 *   Associative array of the POST data, which contains:
 *   - lng
 *       Language of translations.
 *
 * @return
 *   Associative array containing:
 *   - messages
 *       Array of notification messages; each notification message
 *       is an array of a message and a type, where type can be one of
 *       'status', 'warning', 'error'.
 */
function _qtrServices_like_import($params) {
  // Get and check the parameter 'lng'.
  $lng = $params['lng'];
  if (!in_array($lng, qtr::lng_get())) {
    return services_error('Language missing or not supported.', 406);
  }

  // Make sure that the current user can make
  // translations for the given language.
  $user = user_load($GLOBALS['user']->uid);
  if ($lng != $user->translation_lng) {
    $msg = t('No rights for contributing to language <strong>!lng</strong>.', ['!lng' => $lng]);
    return ['messages' => [[$msg, 'error']]];
  }

  // Check access permissions.
  if (!user_access('qtranslate-like')) {
    $msg = t('No rights for submitting likes!');
    return ['messages' => [[$msg, 'error']]];
  }
  if (!user_access('qtranslate-suggest')) {
    $msg = t('No rights for submitting translations!');
    return ['messages' => [[$msg, 'error']]];
  }

  // Get the parameters.
  if (!isset($_FILES['file'])) {
    return services_error('No file uploaded.', 406);
  }
  $uploaded_file = $_FILES['file'];

  // Check that a file is uploaded.
  if (!$uploaded_file['tmp_name']) {
    return services_error('File upload failed.', 406);
  }

  // Check for any other upload errors.
  if ($uploaded_file['error'] != 0) {
    return services_error('Error ' . $uploaded_file['error'] . 'happened during file upload.', 406);
  }

  // Check the extension of the uploaded file.
  $extensions = 'po tar gz tgz bz2 xz 7z zip';
  $regex = '/\.(' . preg_replace('/ +/', '|', preg_quote($extensions)) . ')$/i';
  if (!preg_match($regex, $uploaded_file['name'])) {
    $msg = t('Only files with the following extensions are allowed: %files-allowed.',
           ['%files-allowed' => $extensions]);
    return ['messages' => [[$msg, 'error']]];
  }

  // Move the uploaded file to 'private://' (/var/www/uploads/).
  $file_uri = 'private://' . $uploaded_file['name'];
  if (!drupal_move_uploaded_file($uploaded_file['tmp_name'], $file_uri)) {
    $msg = [t('Failed to move uploaded file.'), 'error'];
    return ['messages' => [$msg]];
  }

  // Save the file data to the DB.
  $file = new stdClass();
  $file->uid = $GLOBALS['user']->uid;
  $file->status = FILE_STATUS_PERMANENT;
  $file->filename = trim(drupal_basename($uploaded_file['name']), '.');
  $file->uri = $file_uri;
  $file->filemime = file_get_mimetype($file->filename);
  $file->filesize = $uploaded_file['size'];
  try {
    $file = file_save($file);
  }
  catch (Exception $e) {
    $msg = [$e->getMessage(), 'error'];
    return ['messages' => [$msg]];
  }

  // Schedule the import.
  $queue_params = [
    'uid' => $GLOBALS['user']->uid,
    'fid' => $file->fid,
    'lng' => $lng,
  ];
  qtr::queue('import_translations', array($queue_params));

  // Return a notification message.
  $msg = t("Import of the file '!filename' is scheduled. You will be notified by email when it is done.",
         ['!filename' => $file->filename]);
  return ['messages' => [[$msg, 'status']]];
}