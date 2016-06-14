#!/bin/bash
### Materialized views are used to speed-up
### term autocompletion of vocabularies.
### Update all the mv tables.

### go to the script directory
cd $(dirname $0)

### mysql and mysqldump options
dbname=${BTR_DATA:-qtr_data}
mysql="mysql --defaults-file=/etc/mysql/debian.cnf --database=$dbname -B"

### drop all 'qtr_mv_*' tables (except 'qtr_mv_sample')
tables=$($mysql -e "SHOW TABLES" | grep '^qtr_mv_' | sed -e '/qtr_mv_sample/d')
for table in $tables
do
    $mysql -e "DROP TABLE IF EXISTS $table"
done

### for each vocabulary create a mv table
vocabularies=$(drush qtr-vocabulary-list | gawk '{print $2 "_" $1}')
for vocabulary in $vocabularies
do
    table="qtr_mv_${vocabulary,,}"
    $mysql -e "CREATE TABLE $table LIKE qtr_mv_sample"
    $mysql -e "INSERT INTO $table
	SELECT DISTINCT s.string FROM qtr_strings s
	JOIN qtr_locations l ON (l.sguid = s.sguid)
	JOIN qtr_templates t ON (t.potid = l.potid)
	JOIN qtr_projects  p ON (p.pguid = t.pguid)
	WHERE p.project = '$vocabulary'
	  AND p.origin = 'vocabulary'
	ORDER BY s.string"
done
