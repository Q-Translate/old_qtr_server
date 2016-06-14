#!/bin/bash -x
### Import B-Translator translations.

### go to the script directory
cd $(dirname $0)

### get config vars
. ../config.sh

### import the POT files
code_dir='/usr/local/src'
$drush qtrp-add Drupal qtr_server $code_dir/qtr_server/l10n/qtrserver.pot
$drush qtrp-add Drupal qtr_client $code_dir/qtr_client/l10n/qtrclient.pot

### import the PO file of each language
for lng in $languages
do
    $drush qtrp-import Drupal qtr_server $lng $code_dir/qtr_server/l10n/qtrserver.$lng.po
    $drush qtrp-import Drupal qtr_client $lng $code_dir/qtr_client/l10n/qtrclient.$lng.po
done
