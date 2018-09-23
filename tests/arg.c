#include <unistd.h>

static int strlen (char *s)
{
	int i = 0;
	while (*s) {
		++i;
		++s;
	}
	return i;
}

int main (int argc, char *argv[])
{
	int i;

	write (1, "Hello!\n", 7);
	for (i = 0; i < argc; ++i) {
		write (1, argv[i], strlen(argv[i]));
		write (1, "\n", 1);
	}
	return 0;
}
