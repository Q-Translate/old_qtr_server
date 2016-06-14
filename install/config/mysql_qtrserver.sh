#!/bin/bash
### set passwords for the mysql users qtr and qtr_data

### get a new password for the mysql user 'qtr'
if [ "$mysql_passwd_qtr" = 'random' ]
then
    mysql_passwd_qtr=$(mcookie | head -c 16)
elif [ -z "${mysql_passwd_qtr+xxx}" -o "$mysql_passwd_qtr" = '' ]
then
    echo
    echo "===> Please enter new password for the MySQL 'qtr' account."
    echo
    mysql_passwd_qtr=$(mcookie | head -c 16)
    stty -echo
    read -p "Enter password [$mysql_passwd_qtr]: " passwd
    stty echo
    echo
    mysql_passwd_qtr=${passwd:-$mysql_passwd_qtr}
fi

### get a new password for the mysql user 'qtr_data'
if [ "$mysql_passwd_qtr_data" = 'random' ]
then
    mysql_passwd_qtr_data=$(mcookie | head -c 16)
elif [ -z "${mysql_passwd_qtr_data+xxx}" -o "$mysql_passwd_qtr_data" = '' ]
then
    echo
    echo "===> Please enter new password for the MySQL 'qtr_data' account."
    echo
    mysql_passwd_qtr_data=$(mcookie | head -c 16)
    stty -echo
    read -p "Enter password [$mysql_passwd_qtr_data]: " passwd
    stty echo
    echo
    mysql_passwd_qtr_data=${passwd:-$mysql_passwd_qtr_data}
fi

### set passwords
source $(dirname $0)/set_mysql_passwd.sh
set_mysql_passwd qtr $mysql_passwd_qtr
set_mysql_passwd qtr_data $mysql_passwd_qtr_data

### modify the configuration file of Drupal (settings.php)
for file in $(ls /var/www/qtr*/sites/default/settings.php)
do
    sed -i $file \
	-e "/^\\\$databases = array/,+10  s/'password' => .*/'password' => '$mysql_passwd_qtr',/" \
	-e "/^\\\$databases\\['qtr_db/,+5  s/'password' => .*/'password' => '$mysql_passwd_qtr_data',/"
done
