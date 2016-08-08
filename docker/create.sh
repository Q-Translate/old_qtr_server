#!/bin/bash -x

### go to the docker directory
cd $(dirname $0)/../

### load the config file
if ! test -f config
then
    echo "File $(pwd)/config is missing."
    exit 1
fi
source ./config

### stop and remove the container if it exists
docker stop $container 2>/dev/null
docker rm $container 2>/dev/null

### Remove the given directory if it exists.
function remove_dir() {
    local dir=$1
    if test -d $dir/
    then
        cd $dir/
        if test -n "$(git status --porcelain)"
        then
            echo "Directory $dir/ cannot be removed because it has uncommited changes."
            exit 1
        fi
        cd ..
        rm -rf $dir/
    fi
}

docker_create() {
    docker create --name=$container --hostname=$hostname \
        --security-opt seccomp=unconfined \
        --stop-signal=SIGRTMIN+3 \
        --tmpfs /run \
        --tmpfs /run/lock \
        -v /sys/fs/cgroup:/sys/fs/cgroup:ro \
        "$@" $ports $image
}

if [ "$dev" = 'false' ]
then
    ### create a container for production
    docker_create \
        --restart=always \
        -v $(pwd)/uploads:/var/www/uploads \
        -v $(pwd)/downloads:/var/www/downloads
else
    ### remove the directories qtr_client/ and qtr_server/ if they exist
    remove_dir qtr_client
    remove_dir qtr_server

    ### copy directories qtr_client/ and qtr_server/ from the image to the host
    docker create --name=$container $image
    docker start $container
    docker cp $container:/var/www/qtr/profiles/qtr_server $(pwd)/
    docker cp $container:/var/www/qcl/profiles/qtr_client $(pwd)/
    docker stop $container
    docker rm $container

    ### create a container for development, sharing diectories
    ### qtr_client/ and qtr_server/ between the container and the host
    docker_create \
        -v $(pwd)/qtr_server:/var/www/qtr/profiles/qtr_server \
        -v $(pwd)/qtr_client:/var/www/qcl/profiles/qtr_client
fi
