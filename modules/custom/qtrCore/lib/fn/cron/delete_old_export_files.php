<?php
/**
 * @file
 * Function: cron_delete_old_export_files()
 */

namespace QTranslate;

/**
 * Delete export files that are older than 2 days
 */
function cron_delete_old_export_files() {
  $export_path = variable_get('qtr_export_path', '/var/www/exports');
  exec("find $export_path/* -mtime +2 -delete");
}
