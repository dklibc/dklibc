#include <termios.h>

/* Return: !=0 - if @fd is tty, 0 - otherwise */
int isatty (int fd)
{
	struct termios t;

	return tcgetattr(fd, &t) != 1;
}
