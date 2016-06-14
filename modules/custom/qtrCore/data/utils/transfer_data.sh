#!/bin/bash

mysql="mysql --defaults-file=/etc/mysql/debian.cnf -B"
db=qtr_data
db1=qtranslate_data
tables=$($mysql -D $db -e "SHOW TABLES" | grep '^qtr_' )
for table in $tables
do
    table1=${table//qtr_/l10n_feedback_}
    echo "$table1 --> $table"
    $mysql -e "
        TRUNCATE TABLE $db.$table;
        INSERT INTO $db.$table SELECT * FROM $db1.$table1;
    "
done

#        CREATE TABLE $db.$table LIKE $db1.$table1;
