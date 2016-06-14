#!/bin/bash

### go to the script directory
cd $(dirname $0)

### set drush root
drush="drush --root=/var/www/qtr"

### set some variables
origin=test
project=LibO-desktop
path=$(pwd)/po_files

### create the project
$drush qtrp-add $origin $project $path/LibO-desktop-fr/

### import the PO files of each language
for lng in fr sq
do
    $drush qtrp-import $origin $project $lng $path/LibO-desktop-$lng/
done
