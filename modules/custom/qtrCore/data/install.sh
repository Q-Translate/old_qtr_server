#!/bin/bash

### go to the script directory
cd $(dirname $0)

### create the DB tables
mysql=$(drush sql-connect)
$mysql < db/qtr_schema.sql

### import chapters and verses
data=$(pwd)
drush eval "qtr::import_chapters('$data/quran/index.xml');"
drush eval "qtr::import_verses('$data/quran/uthmani.xml');"

### add a few english translations
drush qtr-import en pickthall  "$data/translations/en/pickthall.txt"
drush qtr-import en shakir  "$data/translations/en/shakir.txt"
drush qtr-import en yusufali  "$data/translations/en/yusufali.txt"
