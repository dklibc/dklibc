#ifndef _UTIME_H
#define _UTIME_H

#include <sys/types.h>

struct utimbuf {
	time_t atime;
	time_t mtime;
};

/*
Set the atime and mtime of file. In case of @ptimes=NULL does nothing 
*/
int utime (const char *fname, const struct utimbuf *ptimes);

#endif

