#include <termios.h>
#include <sys/ioctl.h>
#include <unistd.h>

int tcgetattr (int fd, struct termios *p)
{
	return ioctl (fd, TCGETS, p);
}

int tcsetattr (int fd, int action, const struct termios *p)
{
	return ioctl (fd, action, (void *)p);
}

int tcgetpgrp (int fd)
{
	int pgid;
	
	if (ioctl (fd, TIOCGPGRP, &pgid) < 0)
		return -1;
	return pgid;
}

int tcsetpgrp (int fd, int *pid)
{
	return ioctl (fd, TIOCSPGRP, pid);
}
