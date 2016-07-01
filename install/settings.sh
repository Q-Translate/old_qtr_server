
### Git branches that will be used.
qtr_git_branch='master'
qcl_git_branch='master'

### Domains of the website.
qtr_domain='qtranslate.org'
qcl_domain='qtranslate.net'

### Drupal 'admin' password.
qtr_admin_passwd='admin'
qcl_admin_passwd='admin'

### Emails from the server are sent through the SMTP
### of a GMAIL account. Give the full email
### of the gmail account, and the password.
gmail_account='MyEmailAddress@gmail.com'
gmail_passwd='gmailpassword'

### Translation languages supported by the Q-Translate Server.
languages='en fr de it sq'

### Translation language of Q-Translate Client.
translation_lng='all'

### Mysql passwords. Leave it as 'random'
### to generate a new one randomly
mysql_passwd_root='random'
mysql_passwd_qcl='random'
mysql_passwd_qtr='random'
mysql_passwd_qtr_data='random'

### Install also extra things that are useful for development.
development='true'

### Login through ssh.
### Only login through private keys is allowed.
### See also this:
###   http://dashohoxha.fs.al/how-to-secure-ubuntu-server/
sshd_port=2201
