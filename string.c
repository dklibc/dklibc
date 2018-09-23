#include <string.h>
#include <ctype.h>
#include <stdio.h> /* For NULL */

char *strncpy (char *to, const char *from, int max)
{
	char *s = to;
	
	while (max) {
		--max;
		if (!(*to++ = *from++))
			break;
	}
	memset (to, '\0', max);
	return to;
}

int __cpystr (char *to, const char *from, int max)
{
	int n = 0;

	while (n < max && (*to++ = *from++))
		++n;
	return n < max ? n : -1;
}

int __begin_with (const char *s, const char *prefix)
{
	while (*s == *prefix && *prefix) {
		++s;
		++prefix;
	}
	return !*prefix;
}

char *strcat (char *p, const char *q)
{
	char *s = p;
	
	p += strlen (p);
	strcpy (p, q);
	return p;
}

int atoi (const char *s)
{	
	int sign = 1, d = 0;
	
	if (*s == '-') {
		sign = -1;
		++s;
	}

	while (isdigit(*s)) {
		d = d * 10 + *s - '0';
		++s;
	}

	return d * sign;
}

int __str2num (const char *s)
{
	int d = 0;
	
	while (isdigit(*s)) {
		d = d * 10 + *s - '0';
		++s;
	}

	if (*s)
		return -1;

	return d;
}

char *strrchr(const char *s, int c)
{
	char *p = NULL;
	
	while (*s) {
		if (*s == c)
			p = (char *)s;
		++s;
	}
	return p;
}
