/*
Character types routines
There is a legacy ASCII variant. In modern systems they must be 
dependant of current locale
*/

#ifndef _CTYPE_H
#define _CTYPE_H

#define islower(c) ((c) >= 'a' && (c) <= 'z')
#define isupper(c) ((c) >= 'A' && (c) <= 'Z')
#define isalpha(c) (islower(c) || isupper(c))
#define isspace(c) ((c) == ' ' || (c) == '\t' || (c) == '\n' || (c) == '\r' || (c) == '\f' || (c) == '\v')
/* isblank() - very useful, but missed in C standard, included in POSIX.1-2001 introduced by BSD */
#define isblank(c) ((c) == ' ' || (c) == '\t')
#define isdigit(c) ((c) >= '0' && (c) <= '9')
#define isalnum(c) (isalpha(c) || isdigit(c))
#define isgraph(c) ((c) > 32 && (c) < 127)
#define isxdigit(c) (isdigit(c) || (c) >= 'a' && (c) <= 'f' || (c) >= 'A' && (c) <= 'F')

#endif

