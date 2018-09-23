/* Terminal I/O settings - interface to terminal driver */

#ifndef _TERMIOS
#define _TERMIOS

#include <sys/ioctl.h>

#define NCCS 19

struct termios {
	unsigned c_iflag; /* Input modes (POSIX) */
	unsigned c_oflag; /* Output modes (POSIX) */
	unsigned c_cflag; /* Control modes (POSIX) */
	unsigned c_lflag; /* Local modes (POSIX) */
	unsigned char c_line; /* Line discipline */
	unsigned char c_cc[NCCS]; /* Control chars (POSIX) */
	unsigned c_ispeed; /* Input speed */
	unsigned c_ospeed; /* Output speed */
};

/* c_cc characters */
#define VINTR 0
#define VQUIT 1
#define VERASE 2
#define VKILL 3
#define VEOF 4
#define VTIME 5
#define VMIN 6
#define VSWTC 7
#define VSTART 8
#define VSTOP 9
#define VSUSP 10
#define VEOL 11
#define VREPRINT 12
#define VDISCARD 13
#define VWERASE 14
#define VLNEXT 15
#define VEOL2 16

/* c_iflag bits */
#define IGNBRK	0000001
#define BRKINT	0000002
#define IGNPAR	0000004
#define PARMRK	0000010
#define INPCK	0000020
#define ISTRIP	0000040
#define INLCR	0000100
#define IGNCR	0000200
#define ICRNL	0000400
#define IUCLC	0001000
#define IXON	0002000
#define IXANY	0004000
#define IXOFF	0010000
#define IMAXBEL	0020000
#define IUTF8	0040000

/* c_oflag bits */
#define OPOST	0000001

/* c_cflag bits */
#define   CS8	0000060

/* c_lflag bits */
#define ISIG	0000001
#define ICANON	0000002
#define XCASE	0000004
#define ECHO	0000010
#define ECHOE	0000020
#define ECHOK	0000040
#define ECHONL	0000100
#define NOFLSH	0000200
#define TOSTOP	0000400
#define ECHOCTL	0001000
#define ECHOPRT	0002000
#define ECHOKE	0004000
#define FLUSHO	0010000
#define PENDIN	0040000
#define IEXTEN	0100000

/* tcsetattr() actions */
/* The changes occured immediately */
#define TCSANOW TCSETS
/* The changes occur after all output written to @fd has been 
transmitted. This  should be used when changing params that affect 
output */
#define TCSADRAIN TCSETSW
/* The changes occur after all output written to @fd has been 
transmitted, and all input that has been received but not read will be 
discarded */
#define TCSAFLUSH TCSETSF

/* 'tc' - terminal control */

/* Get/set terminal attributes */
int tcgetattr (int fd, struct termios *p);
int tcsetattr (int fd, int action, const struct termios *p);

/* Get the foreground process group ID of a given terminal */
int tcgetpgrp (int fd);

/* Set the foreground process group ID of a given terminal */
int tcsetpgrp (int fd, int *pgid);

#endif

