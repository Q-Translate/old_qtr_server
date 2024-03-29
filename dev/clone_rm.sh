#!/bin/bash
### Erase a local clone created by clone.sh

if [ $# -ne 1 ]
then
    echo " * Usage: $0 target

   Deletes the application with root /var/www/<target>
   and with DB named <target>.
"
    exit 1
fi
target=$1

### remove the root directory
rm -rf /var/www/$target

### delete the drush alias
sed -i /etc/drush/local_qtr.aliases.drushrc.php \
    -e "/^\\\$aliases\['$target'\] = /,+5 d"

### drop the database
mysql --defaults-file=/etc/mysql/debian.cnf \
      -e "DROP DATABASE IF EXISTS $target;"

### remove the configuration of apache2
rm -f /etc/apache2/sites-{available,enabled}/$target{,-ssl}.conf

### remove from /etc/hosts
qtr_domain=$(head -n 1 /etc/hosts.conf | cut -d' ' -f3)
sub=${target#*_}
hostname=$sub.$qtr_domain
sed -i /etc/hosts.conf -e "/^127.0.0.1 $hostname/d"
/etc/hosts_update.sh

### restart services
/etc/init.d/mysql restart
/etc/init.d/apache2 restart
