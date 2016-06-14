#!/bin/bash -x

### get the code of qtr_client
cd /usr/local/src/
git clone --branch=$bcl_git_branch https://github.com/B-Translator/qtr_client

### export drupal_dir
export drupal_dir=/var/www/bcl
export drush="drush --root=$drupal_dir"

### go to the directory of scripts
export code_dir=/usr/local/src/qtr_client
cd $code_dir/install/scripts/

### make and install the drupal profile 'qtr_client'
./drupal-make-and-install.sh

### additional configurations related to drupal
./drupal-config.sh

### change back $code_dir to qtr_server
export code_dir=/usr/local/src/qtr_server
