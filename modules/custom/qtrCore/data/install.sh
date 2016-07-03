#!/bin/bash

### go to the script directory
cd $(dirname $0)

### create the DB tables
mysql=$(drush sql-connect)
$mysql < db/qtr_schema.sql

### import chapters, verses, and a few english translations
data='/var/www/data'
drush eval "qtr::import_chapters('$data/quran/index.xml');"
drush eval "qtr::import_verses('$data/quran/uthmani.xml');"
drush eval "qtr::import_translations('en', '$data/translations/en/pickthall.txt');"
drush eval "qtr::import_translations('en', '$data/translations/en/shakir.txt');"
drush eval "qtr::import_translations('en', '$data/translations/en/yusufali.txt');"
