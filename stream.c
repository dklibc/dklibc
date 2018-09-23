/*
I/O streams
Stream has one I/O buffer. When stream is open for write-only or 
read-only - it is not a problem. But when stream is open for reading and 
writing it is a BIG problem. It can help: reposition yourself by fseek 
from SEEK_SET or SEEK_END before first writing after readings (except 
your are appending) or before first reading after writings 
*/

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define MAX_OPEN 20

static FILE file[MAX_OPEN] = {
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // stdin
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // stdout
	{ 2, 0, 0, 0, 0, 0, 0, 1, 0, 0}, // stderr
};

FILE *stdin = &file[0];
FILE *stdout = &file[1];
FILE *stderr = &file[2];

void _init_stream (void)
{
	int i;
	
	if (isatty(1))
		file[1].lnb = 1;

	for (i = 0; i < 3; ++i) {
		file[i].bufsz = BUFSIZ;
		file[i].buf = (char *)malloc (file[i].bufsz);
		file[i].pcur = file[i].buf;
	}
}

void _fin_stream (void)
{
	int i;
	
	for (i = 0; i < MAX_OPEN; ++i) {
		if (file[i].buf)
			fclose (file + i);
	}
}

FILE *fopen (const char *name, const char *mode)
{
	int i;
	FILE *fp;
	
	/*
	"r"  -> O_RDONLY
	"r+" -> O_RDWR, O_CREAT
	"w"  -> O_WRONLY, O_CREAT, O_TRUNC
	"w+" -> O_RDWR, O_CREAT, O_TRUNC		
	"a"  -> O_WRONLY, O_CREAT, O_APPEND
	"a+" -> O_RDWR, O_CREAT, O_APPEND
	*/

	for (i = 0; i < MAX_OPEN; ++i) {
		if (!file[i].buf)
			break;
	}

	if (i == MAX_OPEN) {
		errno = EMFILE;
		return NULL;
	}

	fp = file + i;

	switch (*mode) {
		case 'r':
			i = O_RDONLY;
			break;
		case 'w':
			i = O_WRONLY | O_CREAT | O_TRUNC;
			break;
		case 'a':
			fp->append = 1;
			i = O_WRONLY | O_CREAT | O_APPEND;
			break;
		default:
			errno = EINVAL;
			return 0;
	}

	if (*(mode+1) == '+') {
		i &= ~(O_RDONLY | O_WRONLY);
		i |= O_RDWR | O_CREAT;
	}

	fp->fd = open (name, i, 0666 /* Just in case we are creating a file */);
	if (fp->fd < 0)
		return NULL;

	fp->bufsz = BUFSIZ;
	fp->buf = (char *)malloc (fp->bufsz);
	if (!fp->buf) {
		close (fp->fd);
		return NULL;
	}
	fp->pcur = fp->buf;

	if (isatty (fp->fd))
		fp->lnb = 1;

	return fp;
}

static int _fillbuf (FILE *fp)
{
	int n;

	if (fp->stat)
		return EOF;
	
	fp->pcur = fp->buf;
	fp->n = read (fp->fd, fp->buf, fp->bufsz);
	if (fp->n <= 0) {
		if (!fp->n)
			fp->stat = _STREAM_STAT_EOF;
		else
			fp->stat = _STREAM_STAT_ERR;
		fp->n = 0;
		return EOF;
	}
	return 0;
}

int fflush (FILE *fp)
{
	int n;

	if (!(fp->dwr)) { // Just discard all read data
		fp->pcur = fp->buf;
		fp->n = 0;
		return 0;
	}

	fp->pcur = fp->buf;
	while (fp->n) {
		n = write (fp->fd, fp->pcur, fp->n);
		if (n < 0) {
			fp->stat = _STREAM_STAT_ERR;
			return EOF;
		}
		fp->n -= n;
		fp->pcur += n;
	}
	fp->pcur = fp->buf;
	fp->dwr = 0;
	return 0;
}

int fclose (FILE *fp)
{
	int r;
	
	if (!fp)
		return 0;

	r = fflush (fp);
	free (fp->buf);
	fp->buf = 0;
	return close (fp->fd) + r;
}

int fseek (FILE *fp, int off, int from)
{
	int pos;	
	
	if (fflush (fp) == EOF)
		return -1;
	
	if (from == SEEK_CUR) {
		from = SEEK_SET;
		off += fp->cpos;
	}

	if ((pos = lseek (fp->fd, off, from)) < 0)
		return -1;
	fp->cpos = pos;
	fp->stat = 0; /* Clean possible EOF */
	return 0;
}

/*
Return next char or EOF
*/
int fgetc (FILE *fp)
{
	if (fp->dwr) { /* Read after writes */
		if (fflush(fp) == EOF)
			return EOF;
	}

	if (!fp->n) {
		if (_fillbuf(fp) == EOF)
			return EOF;
	}

	++(fp->cpos);
	--(fp->n);
	return *(unsigned char *)(fp->pcur)++;
}

char *fgets (char *buf, int n, FILE *fp)
{
	int c;
	char *q = buf;
	
	while (--n && (c = fgetc(fp)) != EOF) {
		*buf++ = c;
		if (c == '\n')
			break;
	}

	if (buf == q)
		return NULL;

	*buf = '\0';
	return buf;
}

char *gets (char *buf)
{
	int c;
	char *s = buf;

	while ((c = getchar()) != EOF && c != '\n')
		*s++ = c;
	*s = 0;
	return s == buf ? NULL : buf;
}

// Return: c - OK, EOF - error
int fputc (int c, FILE *fp)
{
	if (!(fp->dwr) && fp->n) {
		fflush (fp); 	/* Discard read data from buffer */
		if (!fp->append) { /* Set file pos to stream pos */
			fseek (fp, fp->cpos, SEEK_SET);
		}
	}

	fp->dwr = 1;
	*(fp->pcur)++ = c;
	++(fp->n);

	/* If file is opened for appending then
	writing doesn't change file position */
	if (!fp->append)
		++(fp->cpos);

	if (fp->n == fp->bufsz || c == '\n' && fp->lnb) {
		if (fflush (fp) < 0)
			return EOF;
	}

	return c;
}

int fputs (const char *s, FILE *fp)
{
	if (fwrite (s, strlen(s), 1, fp) != 1)
		return EOF;
	return 0;
}

int puts (const char *s)
{
	if (fputs (s, stdout) == EOF)
		return EOF;
	return putchar ('\n');
}

/*
Reading data that is greater than buffer is considered rare and not 
optimized (excess copying). So, if you want to load entire file into 
memory buffer use direct read()
*/
int fread (char *buf, int record_sz, int nrecords, FILE *fp)
{
	int n = record_sz * nrecords, k, d;
	
	if (fp->dwr) { /* Read after writes */
		if (fflush (fp) == EOF)
			return EOF;
	}

	if (fp->n >= n) {
		memcpy (buf, fp->pcur, n);
		fp->pcur += n;
		fp->n -= n;
		fp->cpos += n;
		return nrecords;
	}

	d = 0;
	do {
		if (!fp->n) {
			if (_fillbuf(fp) == EOF)
				break;
		}
		k = (n <= fp->n) ? n : fp->n;
		memcpy (buf, fp->pcur, k);
		d += k;
		buf += k;
		n -= k;
		fp->n -= k;
		fp->pcur += k;
		fp->cpos += k;
	} while (n);

	return d / record_sz;
}

/*
Writing data that is greater than buffer size is considered rare and not 
optimized (excess copying). Line buffering is triggered only if NL is at 
the end of the data chunk
*/
int fwrite (const char *buf, int record_sz, int nrecords, FILE *fp)
{
	int n = record_sz * nrecords, k;

	if (!(fp->dwr) && fp->n) {
		fflush (fp); /* Discard read data */
		if (!fp->append) { /* Set file pos to stream pos */
			fseek (fp, fp->cpos, SEEK_SET);
		}
	}

	fp->dwr = 1;

	if (fp->bufsz - fp->n >= n) { // There is enough space in buffer
		memcpy (fp->pcur, buf, n);
		fp->pcur += n;
		fp->n += n;
		fp->cpos += n;
		if (fp->n == fp->bufsz || fp->lnb && *(fp->pcur - 1) == '\n') {
			if (fflush (fp) == EOF)
				return EOF;
		}
		return nrecords;
	}

	do {
		fp->dwr = 1;
		k = (n < (fp->bufsz - fp->n)) ? n : fp->bufsz - fp->n;
		memcpy (fp->pcur, buf, k);
		buf += k;
		n -= k;
		fp->pcur += k;
		fp->n += k;
		fp->cpos += k;
		
		if (fp->n == fp->bufsz) {
			if (fflush (fp) == EOF)
				return 0;
		}
	} while (n);
	
	if (fp->lnb && fp->n && *(fp->pcur - 1) == '\n') {
		if (fflush (fp) == EOF)
			return 0;
	}

	return nrecords;
}

