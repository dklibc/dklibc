#include <stdio.h>

void main (int argc, char *argv[])
{
	char buf[128];
	int i;

	
	printf ("Input your name: ");
	fflush (stdout);
	gets (buf);
	printf ("Hello, %s!\n", buf);
	while(1);
	/* printf ("n=%x, m=%x, s=%s\n", 0x281089, 0x01032005, "Mar 1 2005"); */
}
