#!/bin/bash
### Useful for updating or checking the status of all git repositories.
### For example:
###    dev/git.sh status --short
###    dev/git.sh pull

options=${@:-status --short}
gitrepos="
    /var/www/qcl*/profiles/qtr_client
    /var/www/qtr*/profiles/qtr_server
    /usr/local/src/qtr_*
    /var/www/*/profiles/qtr*/modules/contrib/qtrclient
"
for repo in $gitrepos
do
    echo
    echo "===> $repo"
    cd $repo
    git $options
done
