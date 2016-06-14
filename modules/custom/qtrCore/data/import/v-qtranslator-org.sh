#!/bin/bash
### Import the translations of https://github.com/Q-Translate/vocabulary-jquery

### go to the script directory
cd $(dirname $0)

### get config vars
. ../config.sh

### get the po files
rm -rf $data_root/vocabulary-jquery/
git clone https://github.com/Q-Translate/vocabulary-jquery $data_root/vocabulary-jquery/

### set some variables
origin=dashohoxha
project=v.qtranslator.org

### create the project and import the PO files of each language
echo -e "\n==========> $origin $project"
drush @qtr qtrp-add $origin $project $data_root/vocabulary-jquery/l10n/app.pot
drush @qtr qtrp-import $origin $project sq $data_root/vocabulary-jquery/l10n/po/sq.po
drush @qtr qtrp-import $origin $project de $data_root/vocabulary-jquery/l10n/po/de.po
drush @qtr qtrp-import $origin $project es $data_root/vocabulary-jquery/l10n/po/es.po

### set the author of translations
drush @qtr qtr-vote --user="Dashamir Hoxha" sq $data_root/vocabulary-jquery/l10n/po/sq.po
drush @qtr qtr-vote --user="OpenSrcKansas"  de $data_root/vocabulary-jquery/l10n/po/de.po
drush @qtr qtr-vote --user="jrosgiralt"     es $data_root/vocabulary-jquery/l10n/po/es.po
