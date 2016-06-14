#!/bin/bash
### Backup all the users, suggestions and votes.

### create the backup directory
date=$(date +%Y%m%d)
backup="qtr-backup-$date"
backup_dir="/tmp/$backup"
mkdir -p $backup_dir/

### set mysqldump options
sql_connect=$(drush @qtr sql-connect --database=qtr_db | sed -e 's/^mysql //' -e 's/--database=/--databases /')
mysqldump="mysqldump $sql_connect --skip-add-drop-table --replace"

### backup translations
$mysqldump --tables qtr_translations --where="umail != ''" \
    > $backup_dir/qtr_data.sql

### backup other tables of qtr_data
table_list="
    qtr_votes
    qtr_translations_trash
    qtr_votes_trash
    qtr_users
    qtr_user_project_roles
    qtr_languages
"
$mysqldump --tables $table_list >> $backup_dir/qtr_data.sql

### fix 'CREATE TABLE' on the sql file
sed -i $backup_dir/qtr_data.sql \
    -e 's/CREATE TABLE/CREATE TABLE IF NOT EXISTS/g'

### backup qcl tables
mysqldump=$(drush @qcl sql-connect | sed -e 's/^mysql/mysqldump/' -e 's/--database=/--databases /')
table_list="users users_roles"
$mysqldump --tables $table_list > $backup_dir/qcl.sql

### backup qtr tables
mysqldump=$(drush @qtr sql-connect | sed -e 's/^mysql/mysqldump/' -e 's/--database=/--databases /')
table_list="
    translation_projects
    qtr_languages
    users
    users_roles
    field_data_field_auxiliary_languages
    field_data_field_projects
    field_data_field_translation_lng
    field_revision_field_auxiliary_languages
    field_revision_field_projects
    field_revision_field_translation_lng
    hybridauth_identity
    hybridauth_session
"
$mysqldump --tables $table_list > $backup_dir/qtr.sql

### create an archive
cd /tmp
rm -f $backup.tgz
tar cfz $backup.tgz $backup/
rm -rf $backup
