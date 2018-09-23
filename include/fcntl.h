/*
Flags for open(), fcntl()
*/

#ifndef _FCNTL_H
#define _FCNTL_H

#define O_RDONLY	0
#define O_WRONLY	1
#define O_RDWR		2
#define O_CREAT		0100	/* Not fcntl */
#define O_EXCL		0200	/* Not fcntl */
#define O_TRUNC		01000	/* Not fcntl */
#define O_APPEND	02000
#define O_NONBLOCK	04000

#endif
