/*
Standard buffered I/O of C Lib:
* buffering
* formatting
*/

#ifndef _STDIO_H
#define _STDIO_H

#include <stdarg.h>

// Recommended I/O buffer size
#define BUFSIZ	4096

// EOF encountered
#define _STREAM_STAT_EOF 1
// Error occured	
#define _STREAM_STAT_ERR 2

#define NULL ((void *)0)

typedef struct {
	int fd;
	char *buf;	/* Allocated in open() */
	int bufsz;
	char *pcur;	/* Pointer to the next byte in buffer */
	int n;		/* Number of bytes in buffer */
	int stat;	/* 0 - OK, 1 - EOF, 2 - ERR */
	int dwr;	/* there is data in buffer that must be written to the file */ 
	int lnb;	/* line buffered output */
	int append;	/* file is opened for appending */
	int cpos;	/* current position in file */
} FILE;

extern FILE *stdin;
extern FILE *stdout;
extern FILE *stderr;

#define EOF (-1)
#define ferror(fp) ((fp)->stat == _STREAM_STAT_ERR)
#define feof(fp) ((fp)->stat == _STREAM_STAT_EOF)
#define fileno(fp) ((fp)->fd)
#define ftell(fp) ((fp)->cpos)
#define getchar() fgetc(stdin)
#define putchar(c) fputc(c, stdout)

// Don't change these values (they must be equal to ones in unistd.h)
#define SEEK_SET	0
#define SEEK_CUR	1
#define SEEK_END	2

FILE *fopen (const char *name, const char *mode);
FILE *fdopen (int fd, const char *mode);
int fclose (FILE *fp);
int fflush (FILE *fp);
int fseek (FILE *fp, int off, int from);
int fgetc (FILE *fp);
int fputc (int c, FILE *fp);
char *fgets (char *buf, int n, FILE *fp);
char *gets (char *buf);
int fputs (const char *buf, FILE *fp);
int fread (char *buf, int sz, int n, FILE *fp);
int fwrite (const char *buf, int sz, int n, FILE *fp);
int printf (const char *frmt, ...);
int fprintf (FILE *fp, const char *frmt, ...);
int __getline (char *buf, int bufsz, FILE *fp);

int sprintf (char *buf, const char *frmt, ...);
int snprintf (char *buf, int n, const char *frmt, ...);
int vsnprintf (char *buf, int n, const char *frmt, va_list ap);
int printf (const char *frmt, ...);
int fprintf (FILE *fp, const char *frmt, ...);
int vfptintf (FILE *fp, const char *frmt, va_list ap);

void perror (const char *s);

#endif

