#include <unistd.h>
#include <stdio.h>

int main (int argc, char *argv[])
{
	int c;

	while ((c = getopt (argc, argv, "abcd:ef")) != -1)
		printf ("%c %s\n", c, optarg);
	printf ("optind=%d\n", optind);
	return 0;
}
