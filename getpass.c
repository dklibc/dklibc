#include <unistd.h>
#include <termios.h>
#include <string.h>
#include <limits.h>
#include <fcntl.h>

static char buf[PASS_MAX];

#define NULL ((void *)0)

char *getpass (const char *prompt)
{
	int fd, n, nleft;
	struct termios old, new;
	char *p;
	
	/* Open controlling terminal of process */
	fd = open ("/dev/tty", O_RDWR);
	
	if (fd < 0)
		return NULL;
	
	if (write (fd, prompt, n = strlen(prompt)) != n) {
		close (fd);
		return NULL;
	}
	
	if (tcgetattr (fd, &old) < 0) {
		close (fd);
		return NULL;
	}

	/* Set terminal in a raw mode */

	new = old;
	
	/*
	Echo off, canonical off, no extended functions, no signal 
	chars (^Z, ^C, ...)
	*/
	new.c_lflag &= ~(ECHO | ICANON | ISIG);
	
	/* any number of chars, no timer */
	new.c_cc[VMIN] = 1; new.c_cc[VTIME] = 0;

	if (tcsetattr (fd, TCSANOW, &new) < 0) {
		close (fd);
		return NULL;
	}

	/* Read password */

	p = buf;
	nleft = PASS_MAX;
	
	do {
		n = read (fd, p, nleft);
		if (n <= 0) {
			tcsetattr (fd, TCSANOW, &old);
			close (fd);
			return NULL;
		}
		nleft -= n;
		/* Scan for '\n' or '\r' */
		while (n && *p != '\n' && *p != '\r') {
			--n;
			++p;
		}
	} while (!n);

	*p = '\0';

	tcsetattr (fd, TCSANOW, &old); /* Restore terminal */
	write (fd, "\n", 1);
	close (fd);
	return buf;
}
