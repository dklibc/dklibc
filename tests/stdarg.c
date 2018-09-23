#include <unistd.h>
#include <stdarg.h>

int strlen (char *s)
{
	int i = 0;
	while (*s) {
		++s;
		++i;
	}
	return i;
}

void print (int n, ...)
{
	char *s;
	va_list ap;
	
	va_start(ap, n);
	
	while (n--) {
		s = va_arg(ap, char *);
		write (1, s, strlen(s));
		write (1, "\n", 1);
	}

	va_end (ap);
}

int main (int argc, char *argv[])
{
	if (argc < 3)
		return -1;
	print (3, argv[0], argv[1], argv[2]);
	return 0;
}
