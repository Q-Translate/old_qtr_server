#!/bin/bash
### Restore the backup made with backup.sh

### get the backup file
if [ $# -ne 1 ]
then
    echo "Usage: $0 backup-file.tgz"
    exit 1
fi

backup_file=$1
if ! test -f $backup_file
then
    echo "File '$backup_file' does not exist"
    exit 2
fi

### extract the backup file on /tmp
tar xz -C /tmp/ -f $backup_file
backup_dir=$(ls -dt /tmp/qtr-backup-*/ | head -n 1)

### execute the sql scripts of the backup
drush @qcl sql-query --file=$backup_dir/qcl.sql
drush @qtr sql-query --file=$backup_dir/qtr.sql
$(drush @qtr sql-connect --database=qtr_db) < $backup_dir/qtr_data.sql

### cleanup
rm -rf $backup_dir
