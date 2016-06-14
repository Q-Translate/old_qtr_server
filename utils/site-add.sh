#!/bin/bash -x
### Installs a qtr_server container for a language
### (which contains both qtr_server and qtr_client).

### get the language code and the ssh port
if [ "$1" = '' ]
then
    echo "Usage: $0 lng ssh_port

The parameter 'lng' is the language code (fr/de/it etc.)
The 'ssh_port' is the port that should be used to ssh into
the container (something like 22XY, a port not in use by
any other containers).

"
    exit 1
fi
lng=$1
ssh_port=${2:-2201}

### set some variables
container="qtr-$lng"
qcl_domain="$lng.qtranslator.org"
qtr_domain="qtr-$lng.qtranslator.org"
admin_passwd=$(mcookie | head -c 10)
gmail_account="$lng@qtranslator.org"
gmail_passwd=$(mcookie)
languages=$(echo $lng fr de it es | tr ' ' "\n" | sort -u | tr "\n" ' ')


###################### create a new container #######################

### create a directory for sharing data with the host
mkdir -p /data/containers/$container

### create a new container
docker create --name=$container \
    --hostname=$lng.qtranslator.org \
    -v /data/containers/$container:/data \
    -v /data/PO_files:/data/PO_files:ro \
    -p $ssh_port:2201 \
    qtranslator/qtr_server:v2.3
docker start $container

### update drupal and the code of the application
docker exec $container dev/git.sh pull
docker exec $container dev/drush_up.sh


######### customize settings and reconfigure the container ##########

docker exec $container \
    sed -i /usr/local/src/qtr_server/install/settings.sh \
        -e "/^domain=/ c domain='$qtr_domain'" \
        -e "/^qcl_domain=/ c qcl_domain='$qcl_domain'" \
        -e "/^admin_passwd=/ c admin_passwd='$admin_passwd'" \
        -e "/^qcl_admin_passwd=/ c qcl_admin_passwd='$admin_passwd'" \
        -e "/^gmail_account=/ c gmail_account='$gmail_account'" \
        -e "/^gmail_passwd=/ c gmail_passwd='$gmail_passwd'" \
        -e "/^languages=/ c languages='$languages'" \
        -e "/^translation_lng=/ c translation_lng='$lng'"
docker exec $container install/{config.sh,settings.sh}
docker exec $container bash -c "translation_lng=$lng /usr/local/src/qtr_client/install/config/translation_lng.sh"

############### get the ssh key of the container ####################

docker cp $container:/root/.ssh/id_rsa .
file_rsa=$container.rsa
mv id_rsa $file_rsa
chmod 600 $file_rsa
#gdrive upload -f $file_rsa


################### clean up and restart ############################

docker exec $container \
    killall mysqld
docker exec $container \
    drush @local_qtr --yes cc all
docker exec $container \
    drush @local_qcl --yes cc all

docker restart $container


######## update the configuration of wsproxy and restart it #########

### add on wsproxy apache2 config files for the new site
cd /data/wsproxy/config/etc/apache2/sites-available/
for file in $(ls xmp*.conf)
do
    file1=${file/#xmp/$lng}
    file2=${file/#xmp/qtr-$lng}
    cp $file $file1
    cp $file $file2
    sed -i $file1 -e "s/example\.org/$lng.qtranslator.org/g"
    sed -i $file2 -e "s/example\.org/qtr-$lng.qtranslator.org/g"
done
cd ../sites-enabled/
ln -s ../sites-available/$lng*.conf .
ln -s ../sites-available/qtr-$lng*.conf .
cd /data/

### modify the configuration of wsproxy/hosts.txt
sed -i /data/wsproxy/hosts.txt -e "/^$container:/d"
cat << EOF >> /data/wsproxy/hosts.txt
$container: $lng.qtranslator.org dev.$lng.qtranslator.org test.$lng.qtranslator.org
$container: qtr-$lng.qtranslator.org dev.qtr-$lng.qtranslator.org test.qtr-$lng.qtranslator.org
EOF

### restart wsproxy
/data/wsproxy/restart.sh
docker exec wsproxy /etc/init.d/apache2 restart


####################### import translations #########################

# docker exec $container \
#     /var/www/data/import.sh
