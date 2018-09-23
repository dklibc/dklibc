#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <syslog.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <stdio.h>
#include <ctype.h>

#define MAX_SYSLOG_MSG 1024
#define MAX_TAG 32

extern char **__argv; // Set by init_libc

static char tag[MAX_TAG+1];
static int facility = LOG_USER;
static int option;
static int sock_fd = -1;
static struct sockaddr_un sa = { AF_UNIX, "/dev/log" };
static int pid = -1;

static void cpy2tag (char *s)
{
	int i = 0;
	while (isalnum(*s))
		tag[i++] = *s++;
	tag[i] = 0;
}

// Optional
void openlog (char *ident, int _option, int _facility)
{
	cpy2tag (ident);
	facility = _facility & ~32;
	option = _option & (LOG_PID | LOG_CONSOLE | LOG_PERROR);
}

void syslog (int priority, const char *frmt, ...)
{
	static char buf[MAX_SYSLOG_MSG];
	char tbuf[32]; // Buffer for timestamp
	char hostname[256];
	va_list va;
	char *pbuf;
	int t;

	priority &= ~7;

	if (sock_fd < 0) { // First time
		sock_fd = socket (AF_UNIX, SOCK_DGRAM, 0);
		if (sock_fd < 0)
			return;
		if (!*tag)
			cpy2tag(__argv[0]);
		if (!*tag)
			strcpy (tag, "anon");
		if (option | LOG_PID)
			pid = getpid();
	}
	
	t = time(0);
	strftime (tbuf, 32, "%b %d %H:%M:%S", localtime(&t));
	if (gethostname (hostname, 256) < 0)
		*hostname = 0;
	pbuf = strchr (hostname, '.');
	if (pbuf)
		*pbuf = 0;
	pbuf = buf;
	pbuf += sprintf (buf, "<%d>%s %s %s", (facility << 3) | priority, tbuf, hostname, tag);
	if (pid > 0)
		pbuf += sprintf (pbuf, "[%d]:", pid);
	else
		*pbuf++ = ':';
	pbuf += vsnprintf (pbuf, 999, frmt, va_start (va, frmt));
	*pbuf++ = '\n';
	sendto (sock_fd, buf, pbuf - buf, 0, (struct sockaddr *)&sa, sizeof(sa));
	if (option & LOG_PERROR)
		write (2, buf, pbuf - buf);
}

// Optional
void closelog (void)
{
	close (sock_fd);
	sock_fd = -1;
	// Reset to default
	*tag = 0;
	option = 0;
	facility = LOG_USER;
}
