<?php
/**
 * @file
 * Alter user_profile_form, user_register_form, etc.
 */

/**
 * Implements hook_form_FORM_ID_alter() for user_profile_form.
 */
function qtrUser_form_user_profile_form_alter(&$form, &$form_state, $form_id) {
  // Replace 'field_projects' with a list of subscribed projects.
  $arr_projects = field_get_items('user', $form['#user'], 'field_projects');
  if ($arr_projects) {
    $lng = $form['#user']->translation_lng;
    $base_url = qtr::utils_get_client_url($lng);
    $items = [];
    foreach ($arr_projects as $proj) {
      $project = $proj['value'];
      $url = "$base_url/qtr/project/$project/$lng/dashboard";
      $items[] = "<a href='$url' class='btn btn-default' target='_blank'>$project/$lng</a>";
    }
    $form['field_projects'] = [
      '#type'  => 'fieldset',
      '#title' => t('Subscribed Projects'),
      'subscribed_projects' => [
        '#markup' => implode(' ', $items),
      ],
    ];
  }
  else {
    $form['field_projects']['#access'] = user_access('qtranslate-admin');
  }

  // Hide 'field_auxiliary_languages', it is not useful yet.
  $form['field_auxiliary_languages']['#access'] = FALSE;

  // Add fields for admin_projects and moderate_projects.
  $form['admin'] = array(
    '#type'  => 'fieldset',
    '#title' => t('Fields that can be seen and modified only by an administrator'),
    '#access' => in_array('administrator', $GLOBALS['user']->roles),
    '#weight' => -100,   // display it on the top
  );
  $form['admin']['admin_projects'] = array(
    '#type' => 'textarea',
    '#title' => t('The list of projects that user can administrate'),
    '#description' => t("Set the projects that the user can admin. Enter each project on a separate line, in the form <em>origin/project</em>."),
    '#default_value' => implode("\n", $form_state['user']->admin_projects),
  );
  $form['admin']['moderate_projects'] = array(
    '#type' => 'textarea',
    '#title' => t('The list of projects that user can moderate'),
    '#description' => t("Set the projects that the user can moderate. Enter each project on a separate line, in the form <em>origin/project</em>."),
    '#default_value' => implode("\n", $form_state['user']->moderate_projects),
  );
}

/**
 * Implements hook_form_FORM_ID_alter() for user_register_form.
 */
function qtrUser_form_user_register_form_alter(&$form, &$form_state, $form_id) {
  // Hide 'field_auxiliary_languages', it is not useful yet.
  $form['field_auxiliary_languages']['#access'] = FALSE;

  // Hide the language field if the language is set in the url.
  if (isset($_GET['lng'])) {
      $form['field_translation_lng']['#access'] = FALSE;
  }

  // Register a validation callback function,
  // where we can set the values for language and projects.
  $form['#validate'][] = '_qtrUser_user_register_validate';
}

/**
 * Validation function for user_register_form.
 */
function _qtrUser_user_register_validate($form, &$form_state) {
  $values = &$form_state['values'];

  // Set the value for the language field.
  if (isset($_GET['lng'])) {
      $values['field_translation_lng']['und'][0]['value'] = $_GET['lng'];
  }
}
