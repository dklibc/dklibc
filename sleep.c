#include <unistd.h>
#include <signal.h>
#include <stdio.h> /* For NULL */

static int __alarm;

static void alarm_handler (int sig)
{
}

unsigned sleep (unsigned sec)
{
	unsigned new, prev, left;
	void (*f) (int);
	
	/* Enable alarm signal */
	f = signal(SIGALRM, alarm_handler);
	sigemptyset (&new);
	sigaddset (&new, SIGALRM);
	sigprocmask (SIG_UNBLOCK, &new, &prev);
	
	alarm (sec); /* Set alarm */
	pause(); /* Wait any signal */
	
	left = alarm (0); /* Disable alarm */
	/* Restore alarm signal settings */
	signal(SIGALRM, f);
	sigprocmask (SIG_SETMASK, &prev, NULL);
	return left;
}
