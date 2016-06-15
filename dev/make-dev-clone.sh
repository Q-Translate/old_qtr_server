#!/bin/bash -x

### make a clone of /var/www/qtr to /var/www/qtr_dev
/usr/local/src/qtr_server/dev/clone.sh qtr qtr_dev

### don't redirect http:// to https://
sed -i /etc/apache2/sites-enabled/qtr_dev.conf \
    -e 's/RedirectPermanent/#RedirectPermanent/'

### comment out the configuration of the database 'qtr_db' so that
### the internal test database can be used instead for translations
sed -i /var/www/qtr_dev/sites/default/settings.php \
    -e '/$databases..qtr_db/,+8 s#^/*#//#'

### set $base_url to http://
sed -i /var/www/qtr_dev/sites/default/settings.php \
    -e '/$base_url/ s#https://#http://#'

### add a test user
drush @qtr_dev user-create user1 --password=pass1 \
      --mail='user1@qtranslate.net' > /dev/null 2>&1

### register a test oauth2 client on qtr_server
drush @qtr_dev php-script \
    --script-path=/usr/local/src/qtr_server/install/config \
    register_oauth2_client.php 'test1' '12345' \
    'https://qtranslate.net/oauth2-client-php/authorized.php'
