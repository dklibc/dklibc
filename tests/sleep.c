#include <unistd.h>
#include <signal.h>

void main (void)
{
	unsigned s;
	
	/* Block all signals */
	sigfillset(&s);
	sigprocmask(SIG_SETMASK, &s, 0);
	sleep (5);
}
