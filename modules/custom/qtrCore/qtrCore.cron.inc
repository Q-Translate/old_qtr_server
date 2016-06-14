<?php
/**
 * @file
 * Functions called by cron.
 */

/**
 * Implements hook_cron_queue_info().
 */
function qtrCore_cron_queue_info() {

  $queues['notifications'] = array(
    'worker callback' => 'qtrCore_send_notification_by_email',
    'time' => 240,
  );
  $queues['export_project'] = array(
    'worker callback' => 'qtrCore_export_project',
    'time' => 2000,
  );
  $queues['import_project'] = array(
    'worker callback' => 'qtrCore_import_project',
    'time' => 2000,
  );
  $queues['import_translations'] = array(
    'worker callback' => 'qtrCore_import_translations',
    'time' => 2000,
  );
  $queues['delete_fake_users'] = array(
    'worker callback' => 'qtrCore_delete_fake_user',
    'time' => 30,
  );
  return $queues;
}

/**
 * Implements hook_cron().
 */
function qtrCore_cron() {
  qtrCore_cron_nightly();
}

/**
 * Make sure that cron functions runs only once per night,
 * no matter how often cron is called.
 */
function qtrCore_cron_nightly() {
  // check whether the crontime has been reached yet
  $crontime = variable_get('qtrCore_crontime', '0');
  if (time() < $crontime)  return;

  // set the crontime after the next midnight (after 03:00 AM)
  $crontime = strtotime(date('Y-m-d 02:55')) + 24*60*60;
  variable_set('qtrCore_crontime', $crontime);

  // Call cron functions.
  qtr::cron_delete_old_export_files();
  qtr::cron_cleanup_fake_users();
  qtr::cron_send_strings_for_review();
  qtr::cron_update_mysql_materialized_views();
}

/**
 * The callback function called from cron_queue 'notifications'.
 */
function qtrCore_send_notification_by_email($params) {
  qtr::email_send($params);
}

/**
 * The callback function called from cron_queue 'export_project'.
 */
function qtrCore_export_project($export_params) {
  qtr::cron_export_project($export_params);
}

/**
 * The callback function called from cron_queue 'import_project'.
 */
function qtrCore_import_project($params) {
  qtr::cron_import_project($params);
}

/**
 * The callback function called from cron_queue 'import_translations'.
 */
function qtrCore_import_translations($params) {
  qtr::cron_import_translations($params);
}

/**
 * The callback function called from cron_queue 'delete_fake_users'.
 */
function qtrCore_delete_fake_user($uid) {
  // Delete from the drupal table 'users'
  user_delete($uid);

  // Delete from the table 'qtr_users'
  qtr::db_delete('qtr_users')->condition('uid', $uid)->execute();
}
