<?php
/**
 * @file
 * Resource definition and callbacks for project.
 */

function _qtrServices_vocabulary() {
  $file = array('type' => 'inc', 'module' => 'qtrServices', 'name' => 'resources/vocabulary');
  $params = array(
    'name' => 'params',
    'optional' => FALSE,
    'source' => 'data',
    'description' => 'Array of parameters.',
    'type' => 'array',
  );
  $public_access = array(
    'access callback' => 'user_access',
    'access arguments' => array('access content'),
  );

  // Add a new string to a vocabulary.
  $vocabulary['actions']['add_string'] = array(
    'help' => 'Add a new string to a vocabulary.',
    'file' => $file,
    'callback' => '_qtrServices_vocabulary_add_string',
    'args' => array($params),
    'access callback' => 'user_access',
    'access arguments' => array('qtranslate-suggest'),
  );

  // Delete a string from a vocabulary.
  $vocabulary['actions']['del_string'] = array(
    'help' => 'Delete a string from a vocabulary.',
    'file' => $file,
    'callback' => '_qtrServices_vocabulary_del_string',
    'args' => array($params),
    'access callback' => 'user_access',
    'access arguments' => array('qtranslate-suggest'),
  );

  // Import vocabulary from a text file.
  $vocabulary['actions']['import'] = array(
    'help' => 'Import vocabulary from a text file.',
    'file' => $file,
    'callback' => '_qtrServices_vocabulary_import',
    'args' => array($params),
    'access callback' => 'user_access',
    'access arguments' => array('qtranslate-like'),
  );

  // Delete a vocabulary.
  $vocabulary['actions']['delete'] = array(
    'help' => 'Delete a vocabulary.',
    'file' => $file,
    'callback' => '_qtrServices_vocabulary_delete',
    'args' => array($params),
  ) + $public_access;

  return $vocabulary;
}

/**
 * Add a new string to a vocabulary.
 *
 * @param $params
 *   Associative array of the POST data, which contains:
 *   - name
 *       The name of the vocabulary.
 *   - lng
 *       The language of the vocabulary.
 *   - string
 *       String to be added.
 *   - notify (optional)
 *       If TRUE, notify subscribers about the new string. Default is TRUE.
 *
 * @return
 *   array('vid' => $vid, 'messages' => $messages)
 *   - $vid is the ID of the new string,
 *               or NULL if no string was added
 *   - $messages is an array of notification messages; each notification
 *               message is an array of a message and a type, where
 *               type can be one of 'status', 'warning', 'error'
 */
function _qtrServices_vocabulary_add_string($params) {
  $params += array(
    'name' => NULL,
    'lng' => NULL,
    'string' => NULL,
    'notify' => TRUE,
  );
  $name = check_plain($params['name']);
  $lng = check_plain($params['lng']);
  $string = check_plain($params['string']);
  $uid = $GLOBALS['user']->uid;
  $notify = check_plain($params['notify']);

  // Check access permissions.
  if (!user_access('qtranslate-suggest')) {
    $msg = t('No rights for contributing suggestions!');
    return ['vid' => NULL, 'messages' => [[$msg, 'error']]];
  }
  // Check that the language matches the translation language of the user.
  $account = user_load($GLOBALS['user']->uid);
  if (!user_access('qtranslate-admin') and ($lng != $account->translation_lng)) {
    $msg = t('No rights for contributing to language <strong>!lng</strong>.', ['!lng' => $lng]);
    return ['vid' => NULL, 'messages' => [[$msg, 'error']]];
  }

  $vid = qtr::vocabulary_string_add($name, $lng, $string, $uid, $notify);
  return ['vid' => $vid, 'messages' => []];
}

/**
 * Delete a string from a vocabulary.
 *
 * @param $params
 *   Associative array of the POST data, which contains:
 *   - name
 *       The name of the vocabulary.
 *   - lng
 *       The language of the vocabulary.
 *   - vid
 *       ID of the string to be deleted.
 *
 * @return
 *   Associative array containing:
 *   - messages
 *       Array of notification messages; each notification message
 *       is an array of a message and a type, where type can be one of
 *       'status', 'warning', 'error'.
 */
function _qtrServices_vocabulary_del_string($params) {
  $params += array(
    'name' => NULL,
    'lng' => NULL,
    'vid' => NULL,
  );
  $name = check_plain($params['name']);
  $lng = check_plain($params['lng']);
  $vid = check_plain($params['vid']);
  if (qtr::vocabulary_string_del($name, $lng, $vid)) {
    return ['messages' => []];
  }
  else {
    $msg = t('Insufficient permissions.');
    return ['messages' => [[$msg, 'error']]];
  };
}

/**
 * Create a vocabulary or update an existing one.
 *
 * Terms and translations are uploaded from a text file
 * which has the same format as the exported TXT1 file,
 * where each line is like this:
 *     term <==> translation1 / translation2 / translation3
 *
 * @param $params
 *   Associative array of the POST data, which contains:
 *   - name
 *       The name of the vocabulary.
 *   - lng
 *       The language of the vocabulary.
 *
 * @return
 *   Associative array containing:
 *   - messages
 *       Array of notification messages; each notification message
 *       is an array of a message and a type, where type can be one of
 *       'status', 'warning', 'error'.
 */
function _qtrServices_vocabulary_import($params) {
  // Get the uploaded file.
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
    return services_error('Error ' . $uploaded_file['error'] . ' happened during file upload.', 406);
  }

  // Get the parameters.
  $params += array(
    'lng' => NULL,
    'name' => NULL,
  );
  $lng = check_plain($params['lng']);
  $name = check_plain($params['name']);

  $uid = $GLOBALS['user']->uid;
  $origin = 'vocabulary';
  $project = $name . '_' . $lng;

  if (qtr::project_exists($origin, $project)) {
    // Check the rights for updating an existing project.
    $is_owner = ( qtr::project_owner($origin, $project) == $uid );
    $can_reimport = ( $is_owner or user_access('qtranslate-admin') );
    if (!$can_reimport) {
      $msg = t("Vocabulary '!vocabulary' already exists. You can update only your own vocabularies.",
             ['!vocabulary' => $project]);
      return ['messages' => [[$msg, 'error']]];
    }
  }
  else {
    // Create a new vocabulary.
    qtr::vocabulary_add($name, $lng, $uid);
  }

  // Import the vocabulary.
  qtr::vocabulary_import($name, $lng, $uploaded_file['tmp_name'], $uid);
  return ['messages' => qtr::messages()];
}

/**
 * Delete a vocabulary.
 *
 * @param $params
 *   Associative array of the POST data, which contains:
 *   - name
 *       The name of the vocabulary.
 *   - lng
 *       The language of the vocabulary.
 *
 * @return
 *   Associative array containing:
 *   - message
 *       Notification message, which is an array of a message and a type, where
 *       type can be one of 'status', 'warning', 'error'.
 */
function _qtrServices_vocabulary_delete($params) {
  $params += array(
    'lng' => NULL,
    'name' => NULL,
  );
  $lng = check_plain($params['lng']);
  $name = check_plain($params['name']);

  $uid = $GLOBALS['user']->uid;
  $origin = 'vocabulary';
  $project = $name . '_' . $lng;

  // Check that this vocabulary exists.
  if (!qtr::project_exists($origin, $project)) {
    $msg = t("Vocabulary '!vocabulary' does not exist.", ['!vocabulary' => $project]);
    return ['message' => [$msg, 'error']];
  };

  // Check the rights for deleting this project.
  $is_owner = ( qtr::project_owner($origin, $project) == $uid );
  $can_delete = ( $is_owner or user_access('qtranslate-admin') );
  if (!$can_delete) {
    $msg = t('You can delete only your own vocabularies.');
    return ['message' => [$msg, 'error']];
  }

  // Delete the project.
  qtr::vocabulary_del($name, $lng);
  $msg = t("Vocabulary '!vocabulary' deleted.", ['!vocabulary' => $project]);
  return ['message' => [$msg, 'status']];
}
