#ifndef _STDLIB_H
#define _STDLIB_H

#define NULL ((void *)0)

void *malloc (int n);
void free (void *);

int atoi (const char *s);
void exit (int status);
int __str2num (const char *s);
char *getenv (const char *name);

#endif
