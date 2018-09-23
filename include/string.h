#ifndef _STRING_H
#define _STRING_H

int strlen (const char *s);
int strcmp (const char *s, const char *q);
char *strcpy (char *to, const char *from);
char *strchr (const char *s, int c);
char *strrchr (const char *s, int c);
char *strncpy (char *to, const char *from, int max);
char *strcat (char *p, const char *q);
void *memset (void *s, int c, int n);
void *memcpy (void *to, const void *from, int n);
int memcmp (const void *s, const void *q, int n);
char *strerror (int errno);
int __cpystr(char *to, const char *from, int n);
int __begin_with (const char *s, const char *prefix);

#endif

