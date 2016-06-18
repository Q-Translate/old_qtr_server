#!/bin/bash
### If it happens that the number of votes for each translation
### becomes inconsistent for some reason, we can update and fix it
### by running this maintenance script.
### It will recount all the votes for each translation and update
### the field 'count' on table 'qtr_translations'

### mysqldump options
dbname=${QTR_DATA:-qtr_data}
mysql="mysql --defaults-file=/etc/mysql/debian.cnf --database=$dbname"

### recount the number of votes and update translations
$mysql -e "
    CREATE TEMPORARY TABLE tmp_counts AS (
        SELECT T1.tguid, count(*) AS count
        FROM qtr_translations T1
        INNER JOIN qtr_votes T2 ON (T1.tguid = T2.tguid)
        GROUP BY T1.tguid
    );

    UPDATE qtr_translations T1
    INNER JOIN tmp_counts T2 ON (T1.tguid = T2.tguid)
    SET T1.count = T2.count;
"
