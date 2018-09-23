#include <unistd.h>
#include <stdio.h>

void main (void)
{
	char *p;
	
	p = getpass("Password: ");
	if (!p)
		perror ("Failed to read password");
	else
		puts (p);
}
