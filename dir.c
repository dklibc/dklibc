#include <dirent.h>
#include <limits.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>

#define MAX_OPEN_DIRS 20
#define DIR_BUF_SZ 1024

static DIR dir[MAX_OPEN_DIRS];

DIR *opendir (const char *fname)
{
	int i;
	struct stat st;

	for (i = 0; i < MAX_OPEN_DIRS && dir[i].buf; ++i);

	if (i == MAX_OPEN_DIRS) {
		errno = ENFILE;
		return NULL;
	}

	if ((dir[i].fd = open (fname, O_RDONLY)) < 0)
		return NULL;

	if (fstat(dir[i].fd, &st)) {
m1:		close(dir[i].fd);
		return NULL;
	}

	if (!S_ISDIR(st.st_mode)) {
		errno = ENOTDIR;
		goto m1;
	}
	
	if (!(dir[i].buf = (char *)malloc (DIR_BUF_SZ)))
		goto m1;

	dir[i].pcur = dir[i].buf;
	dir[i].n = 0;
	return dir + i;
}

/*
Linux syscall to read directory. Can read several entries at once. 
Replace old readdir syscall
*/
int getdents (int fd, struct dirent *p, int sz);

struct dirent *readdir (DIR *p)
{
	struct dirent *q;
	
	if (!(p->n)) {
		if ((p->n = getdents (p->fd, (struct dirent *)p->buf, DIR_BUF_SZ)) <= 0)
			return NULL;
		p->pcur = p->buf;
	}
	
	q = (struct dirent *)p->pcur;
	p->pcur += q->d_reclen;
	p->n -= q->d_reclen;
	return q;
}

void closedir (DIR *p)
{
	close (p->fd);
	free (p->buf);
	p->buf = 0;
}
