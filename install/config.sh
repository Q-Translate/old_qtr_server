#!/bin/bash

### get config settings from a file
if [ "$1" != '' ]
then
    settings=$1
    set -a
    source  $settings
    set +a
fi

### install dirs of the qtr_server and qtr_client
qtr=/usr/local/src/qtr_server/install
qcl=/usr/local/src/qtr_client/install

### remove dev sites
test -d /var/www/qtr_dev && $qtr/../dev/clone_rm.sh qtr_dev
test -d /var/www/qcl_dev && $qcl/../dev/clone_rm.sh qcl_dev

### configure domains
$qtr/config/domain.sh

### set new passwords for mysql users
$qtr/config/mysql_passwords.sh
$qcl/config/mysql_qtrclient.sh
$qtr/config/mysql_qtrserver.sh

### setup SMTP through a gmail account
$qtr/config/gmailsmtp.sh

### set new password for drupal user 'admin'
### on qtr_server and qtr_client
$qcl/config/drupalpass.sh
$qtr/config/drupalpass.sh

### configurations for oauth2 login
$qtr/config/oauth2_login.sh @qcl @qtr

### configure languages
$qcl/config/translation_lng.sh
$qtr/config/languages.sh

### regenerate ssh keys
$qtr/config/ssh_keys.sh

### make clones qtr_dev and qcl_dev
if [ "$development" = 'true' ]
then
    $qtr/../dev/make-dev-clone.sh
    $qcl/../dev/make-dev-clone.sh
    $qtr/config/oauth2_login.sh @qcl_dev @qtr_dev
fi

### drush may create some files with wrong permissions, fix them
$qtr/config/fix_file_permissions.sh
