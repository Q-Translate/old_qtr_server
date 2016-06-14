#!/bin/bash

echo "
===> Set the domain names (fqdn)

These are the domains that you have (or plan to get)
for the qtr_server and for the qtr_client.

It will modify the files:
 1) /etc/hostname
 2) /etc/hosts
 3) /etc/nginx/sites-available/bcl*
 4) /etc/nginx/sites-available/qtr*
 5) /etc/apache2/sites-available/bcl*
 6) /etc/apache2/sites-available/qtr*
 7) /var/www/bcl*/sites/default/settings.php
 8) /var/www/qtr*/sites/default/settings.php
"

### get the current domains
old_bcl_domain=$(head -n 1 /etc/hosts.conf | cut -d' ' -f2)
old_bcl_domain=${old_bcl_domain:-example.org}
old_qtr_domain=$(head -n 1 /etc/hosts.conf | cut -d' ' -f3)
old_qtr_domain=${old_qtr_domain:-qtr.example.org}

### get the new domains
if [ -z "${bcl_domain+xxx}" -o "$bcl_domain" = '' ]
then
    read -p "Enter the domain name for qtr_client [$old_bcl_domain]: " input
    bcl_domain=${input:-$old_bcl_domain}
fi

if [ -z "${qtr_domain+xxx}" -o "$qtr_domain" = '' ]
then
    read -p "Enter the domain name for qtr_server [$old_qtr_domain]: " input
    qtr_domain=${input:-$old_qtr_domain}
fi

### update /etc/hostname and /etc/hosts.conf
echo $bcl_domain > /etc/hostname
sed -i /etc/hosts.conf \
    -e "1c 127.0.0.1 $bcl_domain $qtr_domain" \
    -e "/127.0.0.1 dev.$old_bcl_domain/c 127.0.0.1 dev.$bcl_domain/" \
    -e "/127.0.0.1 dev.$old_qtr_domain/c 127.0.0.1 dev.$qtr_domain/"
/etc/hosts_update.sh

### update config files for the client
for file in $(ls /etc/nginx/sites-available/bcl*)
do
    sed -i $file -e "/server_name/ s/$old_bcl_domain/$bcl_domain/"
done
for file in $(ls /etc/apache2/sites-available/bcl*)
do
    sed -i $file \
        -e "/ServerName/ s/$old_bcl_domain/$bcl_domain/" \
        -e "/RedirectPermanent/ s/$old_bcl_domain/$bcl_domain/"
done
for file in $(ls /var/www/bcl*/sites/default/settings.php)
do
    sed -i $file -e "/^\\\$base_url/ s/$old_bcl_domain/$bcl_domain/"
done

### update config files for the server
for file in $(ls /etc/nginx/sites-available/qtr*)
do
    sed -i $file -e "/server_name/ s/$old_qtr_domain/$qtr_domain/"
done
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
sed -i /etc/drush/local_bcl.aliases.drushrc.php \
    -e "/'uri'/ s/$old_bcl_domain/$bcl_domain/"
sed -i /etc/drush/local_qtr.aliases.drushrc.php \
    -e "/'uri'/ s/$old_qtr_domain/$qtr_domain/"
