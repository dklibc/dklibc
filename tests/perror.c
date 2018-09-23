#include <stdio.h>
#include <errno.h>

void main (void)
{
	errno = EIO;
	perror ("Failed to read file");
}
