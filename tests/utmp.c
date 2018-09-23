#include <stdio.h>
#include <utmp.h>
#include <time.h>

static char *tname(int type)
{
	static char *t[9] = {"Empty", "?", "Boot Time", "?", "?", 
"Init Proc", "Login Proc", "User Proc", "Dead Proc"};	
	return type >= 0 && type < 9 ? t[type] : "?";
}

void main (void)
{
	struct utmp *ut;

	setutent();
	while (ut = getutent()) {
		printf ("\nType: %s", tname(ut->ut_type));
		printf ("\nPID:   %d", ut->ut_pid);
		printf ("\nID:   %d", ut->ut_id);
		printf ("\nLine: %s", ut->ut_line);
		printf ("\nUser: %s", ut->ut_user);
		printf ("\nTime: %s\n", asctime(localtime(&ut->ut_tv.sec)));
	}
	endutent();
}
