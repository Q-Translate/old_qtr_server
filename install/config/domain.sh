#!/bin/bash

echo "
===> Set the domain names (fqdn)

These are the domains that you have (or plan to get)
for the qtr_server and for the qtr_client.

It will modify the files:
 1) /etc/hostname
 2) /etc/hosts
 5) /etc/apache2/sites-available/qcl*
 6) /etc/apache2/sites-available/qtr*
 7) /var/www/qcl*/sites/default/settings.php
 8) /var/www/qtr*/sites/default/settings.php
"

### get the current domains
old_qcl_domain=$(head -n 1 /etc/hosts.conf | cut -d' ' -f2)
old_qcl_domain=${old_qcl_domain:-qtranslate.net}
old_qtr_domain=$(head -n 1 /etc/hosts.conf | cut -d' ' -f3)
old_qtr_domain=${old_qtr_domain:-qtranslate.org}

### get the new domains
if [ -z "${qcl_domain+xxx}" -o "$qcl_domain" = '' ]
then
    read -p "Enter the domain name for qtr_client [$old_qcl_domain]: " input
    qcl_domain=${input:-$old_qcl_domain}
fi

if [ -z "${qtr_domain+xxx}" -o "$qtr_domain" = '' ]
then
    read -p "Enter the domain name for qtr_server [$old_qtr_domain]: " input
    qtr_domain=${input:-$old_qtr_domain}
fi

### update /etc/hostname and /etc/hosts.conf
echo $qcl_domain > /etc/hostname
sed -i /etc/hosts.conf \
    -e "1c 127.0.0.1 $qcl_domain $qtr_domain" \
    -e "/127.0.0.1 dev.$old_qcl_domain/c 127.0.0.1 dev.$qcl_domain/" \
    -e "/127.0.0.1 dev.$old_qtr_domain/c 127.0.0.1 dev.$qtr_domain/"
/etc/hosts_update.sh

### update config files for the client
for file in $(ls /etc/apache2/sites-available/qcl*)
do
    sed -i $file \
        -e "/ServerName/ s/$old_qcl_domain/$qcl_domain/" \
        -e "/RedirectPermanent/ s/$old_qcl_domain/$qcl_domain/"
done
for file in $(ls /var/www/qcl*/sites/default/settings.php)
do
    sed -i $file -e "/^\\\$base_url/ s/$old_qcl_domain/$qcl_domain/"
done

### update config files for the server
for file in $(ls /etc/apache2/sites-available/qtr*)
do
    sed -i $file \
        -e "/ServerName/ s/$old_qtr_domain/$qtr_domain/" \
        -e "/RedirectPermanent/ s/$old_qtr_domain/$qtr_domain/"
done
for file in $(ls /var/www/qtr*/sites/default/settings.php)
do
    sed -i $file -e "/^\\\$base_url/ s/$old_qtr_domain/$qtr_domain/"
done

### update uri on drush aliases
sed -i /etc/drush/local_qcl.aliases.drushrc.php \
    -e "/'uri'/ s/$old_qcl_domain/$qcl_domain/"
sed -i /etc/drush/local_qtr.aliases.drushrc.php \
    -e "/'uri'/ s/$old_qtr_domain/$qtr_domain/"
