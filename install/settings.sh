
### Git branches that will be used.
qtr_git_branch='master'
qcl_git_branch='master'

### Domains of the website.
qtr_domain='qtr.example.org'
qcl_domain='l10n.example.org'

### Drupal 'admin' password.
qtr_admin_passwd='admin'
qcl_admin_passwd='admin'

### Emails from the server are sent through the SMTP
### of a GMAIL account. Give the full email
### of the gmail account, and the password.
gmail_account='MyEmailAddress@gmail.com'
gmail_passwd='gmailpassword'

### Translation languages supported by the Q-Translate Server.
### Do not remove 'fr', because sometimes French translations
### are used instead of template files (when they are missing).
languages='fr de it sq'

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
###   http://dashohoxha.blogspot.com/2012/08/how-to-secure-ubuntu-server.html
sshd_port=2201
