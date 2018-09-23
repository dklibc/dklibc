#include <string.h>

extern char **environ;

char *getenv (const char *name)
{
	char **p, *q;
	int i;

	i = strlen(name);
	p = environ;
	
	while (*p) {
		q = strchr(*p, '=');
		if (!q) /* Is it possible? */
			continue;
		if ((q - *p == i) && !memcmp(*p, name, i))
			return q + 1;
		++p;
	}
	return *p;
}
