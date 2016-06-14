#!/bin/bash -x

### make sure that we have the right git branch on the make file
makefile="$code_dir/build-qtrserver.make"
sed -i $makefile \
    -e "/qtr_server..download..branch/ c projects[qtr_server][download][branch] = $qtr_git_branch"

### retrieve all the projects/modules and build the application directory
rm -rf $drupal_dir
drush make --prepare-install --force-complete \
           --contrib-destination=profiles/qtr_server \
           $makefile $drupal_dir

### copy the bootstrap library to the custom theme, etc.
cd $drupal_dir/profiles/qtr_server/
cp -a libraries/bootstrap themes/contrib/bootstrap/
cp -a libraries/bootstrap themes/qtr_server/
cp libraries/bootstrap/less/variables.less themes/qtr_server/less/

### copy hybridauth provider GitHub.php to the right place
cd $drupal_dir/profiles/qtr_server/libraries/hybridauth/
cp additional-providers/hybridauth-github/Providers/GitHub.php \
   hybridauth/Hybrid/Providers/

### replace the profile qtr_server with a version
### that is a git clone, so that any updates
### can be retrieved easily (without having to
### reinstall the whole application).
cd $drupal_dir/profiles/
mv qtr_server qtr_server-bak
cp -a $code_dir .
### copy contrib libraries and modules
cp -a qtr_server-bak/libraries/ qtr_server/
cp -a qtr_server-bak/modules/contrib/ qtr_server/modules/
cp -a qtr_server-bak/themes/contrib/ qtr_server/themes/
### cleanup
rm -rf qtr_server-bak/

### create the directory of PO files
mkdir -p /var/www/PO_files
chown www-data: /var/www/PO_files

### create the downloads and exports dirs
mkdir -p /var/www/downloads/
chown www-data: /var/www/downloads/
mkdir -p /var/www/exports/
chown www-data: /var/www/exports/
mkdir -p /var/www/uploads/
chown www-data: /var/www/uploads/

### start mysqld manually, if it is not running
if test -z "$(ps ax | grep [m]ysqld)"
then
    nohup mysqld --user mysql >/dev/null 2>/dev/null &
    sleep 5  # give time mysqld to start
fi

### settings for the database and the drupal site
db_name=qtr
db_user=qtr
db_pass=qtr
site_name="B-Translator"
site_mail="$gmail_account"
account_name=admin
account_pass="$qtr_admin_passwd"
account_mail="$gmail_account"

### create the database and user
mysql='mysql --defaults-file=/etc/mysql/debian.cnf'
$mysql -e "
    DROP DATABASE IF EXISTS $db_name;
    CREATE DATABASE $db_name;
    GRANT ALL ON $db_name.* TO $db_user@localhost IDENTIFIED BY '$db_pass';
"

### site installation
sed -e '/memory_limit/ c memory_limit = -1' -i /etc/php5/cli/php.ini
cd $drupal_dir
drush site-install --verbose --yes qtr_server \
      --db-url="mysql://$db_user:$db_pass@localhost/$db_name" \
      --site-name="$site_name" --site-mail="$site_mail" \
      --account-name="$account_name" --account-pass="$account_pass" --account-mail="$account_mail"

### install multi-language support
mkdir -p $drupal_dir/sites/all/translations
chown -R www-data: $drupal_dir/sites/all/translations

### set the list of supported languages
sed -i $drupal_dir/profiles/qtr_server/modules/custom/qtrCore/data/config.sh \
    -e "/^languages=/c languages=\"$languages\""
drush --root=$drupal_dir --yes vset qtr_languages "$languages"

### add these languages to drupal
drush dl drush_language
for lng in $languages
do
    drush --root=$drupal_dir language-add $lng
done

### fix tha DB schema and install some test data
$drupal_dir/profiles/qtr_server/modules/custom/qtrCore/data/install.sh

### set propper directory permissions
mkdir -p sites/default/files/
chown -R www-data: sites/default/files/
mkdir -p cache/
chown -R www-data: cache/

