#!/bin/bash

### get the aliases for the client and for the server
if [ $# -ne 2 ]
then
    echo "Usage: $0 @qcl_alias @qtr_alias"
    exit 1
fi
qcl_alias=$1
qtr_alias=$2

echo "===> Making configurations for OAuth2 Login"

### get client url and server url
qcl_url=$(drush $qcl_alias php-eval 'print $GLOBALS["base_url"]')
qtr_url=$(drush $qtr_alias php-eval 'print $GLOBALS["base_url"]')

### configuration variables
server_url=$qtr_url
client_id='localclient'
client_secret=$(mcookie)
redirect_url=$qcl_url/oauth2/authorized
skip_ssl=1

### register an oauth2 client on qtr_server
qtr=/usr/local/src/qtr_server/install/config
drush --yes $qtr_alias \
    php-script --script-path=$qtr register_oauth2_client.php  \
    "$client_id" "$client_secret" "$redirect_url"
drush $qtr_alias cc all

### setup oauth2 login on qtr_client
qcl=/usr/local/src/qtr_client/install/config
drush --yes $qcl_alias \
    php-script --script-path=$qcl oauth2_login.php  \
    "$server_url" "$client_id" "$client_secret" "$skip_ssl"
drush $qcl_alias cc all

### set the variable qtr_client of the server
drush --yes $qtr_alias vset qtr_client "$qcl_url"
