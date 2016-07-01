#!/bin/bash
### Update all projects at once.

for dir in /var/www/qtr*
do
    echo
    echo "===> $dir"
    cd $dir
    drush vset update_check_disabled 1 -y
    drush pm-refresh
    drush up -y
done

for dir in /var/www/qcl*
do
    echo
    echo "===> $dir"
    cd $dir
    drush vset update_check_disabled 1 -y
    drush pm-refresh
    drush up -y
done
