#include <time.h>
#include <stdio.h>

void main (void)
{
	time_t t;
	struct tm *p;
	char buf[256];

	t = time (0);
	p = localtime (&t);
	printf ("%d\n", t);
	strftime (buf, 256, "%a %b %d %H:%M:%S %Y", p);
	puts (buf);
}
