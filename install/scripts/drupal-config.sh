#!/bin/bash -x

# Copy the logo file to the drupal dir.
cp $drupal_dir/profiles/qtr_server/qtr_logo.png $qrupal_dir/logo.png

# Protect Drupal settings from prying eyes
drupal_settings=$drupal_dir/sites/default/settings.php
chown root:www-data $drupal_settings
chmod 640 $drupal_settings

### Modify Drupal settings

# disable poor man's cron
cat >> $drupal_settings << EOF
/**
 * Disable Poor Man's Cron:
 *
 * Drupal 7 enables the built-in Poor Man's Cron by default.
 * Poor Man's Cron relies on site activity to trigger Drupal's cron,
 * and is not well suited for low activity websites.
 *
 * We will use the Linux system cron and override Poor Man's Cron
 * by setting the cron_safe_threshold to 0.
 *
 * To re-enable Poor Man's Cron:
 *    Comment out (add a leading hash sign) the line below,
 *    and the system cron in /etc/cron.d/drupal7.
 */
\$conf['cron_safe_threshold'] = 0;

EOF

# set base_url
cat >> $drupal_settings << EOF
\$base_url = "https://qtranslate.org";

EOF

### set variable qtr_client
$drush --yes vset qtr_client "https://$qcl_domain"

### set the directory for uploads
### $drush is an alias for 'drush --root=/var/www/qtr'
mkdir -p /var/www/uploads/
chown www-data: /var/www/uploads/
$drush variable-set file_private_path '/var/www/uploads/' --exact --yes

### install features
$drush --yes pm-enable qtr_qtrServices
$drush --yes features-revert qtr_qtrServices

$drush --yes pm-enable qtr_qtr
$drush --yes features-revert qtr_qtr

$drush --yes pm-enable qtr_misc
$drush --yes features-revert qtr_misc

$drush --yes pm-enable qtr_layout
$drush --yes features-revert qtr_layout

$drush --yes pm-enable qtr_hybridauth
$drush --yes features-revert qtr_hybridauth

#$drush --yes pm-enable qtr_captcha
#$drush --yes features-revert qtr_captcha

$drush --yes pm-enable qtr_permissions
$drush --yes features-revert qtr_permissions

### update to the latest version of core and modules
#$drush --yes pm-refresh
#$drush --yes pm-update

### refresh and update translations
if [ "$development" != 'true' ]
then
    $drush --yes l10n-update-refresh
    $drush --yes l10n-update
fi
