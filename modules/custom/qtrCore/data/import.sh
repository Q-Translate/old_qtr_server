#!/bin/bash

### get the language code
if [[ -z $1 ]]; then
    echo "
Usage: $0 <lng>

where <lng> is the language code, like: en, fr, it, de, sq, etc.
"
    exit 1
fi
lng=$1

### go to the directory of translations
cd $(dirname $0)
mkdir -p translations/
cd translations/

### get and import the translations of the given language
translations='https://github.com/Q-Translate/translations/raw/master'
files=$(wget -qO- $translations/LIST.txt | grep "^$lng\.")
for file in $files; do
    echo "Get $file"
    rm -f $file
    wget -q $translations/$file

    echo "Import $file"
    author=${file%.txt}
    drush qtr-import $lng $author "$(pwd)/$file" --force
done
