/* ioctl commands */

#ifndef _SYS_IOCTL_H
#define _SYS_IOCTL_H

/* ***** Terminal ***** */

/* Get/set termios struct of a given terminal */
#define TCGETS 0x5401
/* equiv to tcsetattr(fd, TCSANOW, p) */
#define TCSETS 0x5402
/* equiv to tcsetattr(fd, TCSADRAIN, p) */
#define TCSETSW 0x5403
/* equiv to tcsetattr(fd, TCSAFLUSH, p) */
#define TCSETSF 0x5404

/*
Make the given terminal the controlling terminal of the calling 
process. The calling process must be a session leader and not have a 
controlling terminal already. If the terminal is already a controlling 
terminal of a different session group then ioctl fails with EPERM
*/
#define TIOCSCTTY 0x540e

/*
If the given terminal is a controlling terminal of the calling process, 
give up the controlling terminal. If the process is a session leader, 
then sent SIGHUP and SIGCONT to the foreground process group and all 
processes in the current session lose their controlling terminal  
*/
#define TIOCNOTTY 0x5422

/* Get the PGID of the foreground process group on this terminal */
#define TIOCGPGRP 0x540f

/* Set the foreground process group ID of this terminal */
#define TIOCSPGRP 0x5410

/* Get the session ID of the given terminal */
#define TIOCGSID 0x5429

#endif

