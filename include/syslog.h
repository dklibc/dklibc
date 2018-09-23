#ifndef _SYSLOG_H
#define _SYSLOG_H

// Priorities
#define LOG_EMERG	0
#define LOG_ALERT	1
#define LOG_CRIT	2
#define LOG_ERR		3
#define LOG_WARNING	4
#define LOG_NOTICE	5
#define LOG_INFO	6
#define LOG_DEBUG	7

// Options
#define LOG_PID		1
#define LOG_PERROR	2
#define LOG_CONSOLE	4

// Facilities
#define LOG_KERN	0
#define LOG_USER	1
#define LOG_MAIL	2
#define LOG_DAEMON	3
#define LOG_AUTH	4
#define LOG_SYSLOG	5
#define LOG_LPR		6
#define LOG_NEWS	7
#define LOG_UUCP	8
#define LOG_AUTHPRIV	10
#define LOG_FTP		11
#define LOG_CRON	15
#define LOG_LOCAL0	16
#define LOG_LOCAL1	17
#define LOG_LOCAL2	18
#define LOG_LOCAL3	19
#define LOG_LOCAL4	20
#define LOG_LOCAL5	21
#define LOG_LOCAL6	22
#define LOG_LOCAL7	23

void openlog (char *, int, int);
void closelog (void);
void syslog (int, const char *, ...);

#endif
