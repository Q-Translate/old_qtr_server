#!/bin/bash
### Set the admin password of Drupal.

### get a password for the Drupal user 'admin'
if [ -z "${qtr_admin_passwd+xxx}" -o "$qtr_admin_passwd" = '' ]
then
    base_url=$(drush @qtr eval 'print $GLOBALS["base_url"]')
    echo
    echo "===> Password for Drupal 'admin' on $base_url."
    echo
    stty -echo
    read -p "Enter the password: " qtr_admin_passwd
    stty echo
    echo
fi

### set the password
drush @qtr user-password admin --password="$qtr_admin_passwd"
