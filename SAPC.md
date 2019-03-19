# Use rsyslog to log output instead of using printf()

### Synopsis
Since the output is going to be on a screen, any output messages should be logged into a file instead of printing them out

### Description
#### Part I:
Create a configuration file that is going to be added to /usr/sbin/rsyslog. /usr/sbin/rsyslog -f [configuration file]

#### Part II:
Figure out how to have /usr/sbin/rsyslog always start with the configuration file that was created for it in Part I
