FROM ubuntu:16.04
RUN systemctl set-default -f multi-user.target
COPY docker/init.sh /usr/local/sbin/
CMD ["/usr/local/sbin/init.sh"]

### Install packages.
COPY install/packages.sh /tmp/
RUN DEBIAN_FRONTEND=noninteractive /tmp/packages.sh

### Copy the source code and install.
COPY . /usr/local/src/qtr_server/
ENV code_dir /usr/local/src/qtr_server
WORKDIR /usr/local/src/qtr_server/
RUN ["install/install.sh"]

### Set the default dir to the code of the application.
WORKDIR /var/www/qtr/profiles/qtr_server
