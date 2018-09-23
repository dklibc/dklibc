#include <syslog.h>

void main (void)
{
	openlog("test", LOG_PERROR, LOG_USER);
	syslog(LOG_INFO, "Hello, world!");
	syslog(LOG_INFO, "I'm working!");
	closelog();
}
