#!/bin/bash -x
### Install and config the system inside a docker container.

### get options and settings
set -a
source options.sh
set +a

### start mysql, in case it is not running
/etc/init.d/mysql start

### go to the directory of scripts
cd $code_dir/install/scripts/

### make and install the drupal profile
export DEBIAN_FRONTEND=noninteractive
export drupal_dir=/var/www/qtr
export drush="drush --root=$drupal_dir"
./drupal-make-and-install.sh

### move translation tables on their own database
./separate-qtr-data.sh

### additional configurations related to drupal
./drupal-config.sh

### install qtr_client as well
./install-qtr-client.sh

### system level configuration (services etc.)
./system-config.sh

### qtranslate configuration
$code_dir/install/config.sh
