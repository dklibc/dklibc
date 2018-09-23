/* POSIX interface for reading directory */

#ifndef _DIRENT_H
#define _DIRENT_H

#include <limits.h>

/*
Directory entry
Only @d_name field is required by POSIX, others fields are non-standard 
and shouldn't be used by program
*/
struct dirent {
	unsigned d_ino;
	unsigned d_off; /* offset to the next entry from the dir start */
	unsigned short d_reclen; /* this record len */
	char d_name[1]; /* null-terminated name, its len is @d_reclen - offset(d_name) - 2 */
	/*
	char pad; // zero padding byte
	char d_type; // file type at the end of record (d_reclen - 1), since Linux 2.6 
	*/
};

typedef struct {
	int fd;
	char *buf;
	char *pcur;
	int n;
} DIR;

DIR *opendir(const char *fname);
void closedir(DIR *p);
struct dirent *readdir(DIR *p);

#endif

