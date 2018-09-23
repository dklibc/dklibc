#include <stdio.h>
#include <grp.h>

void main (void)
{
	struct group *p;
	int i;
	
	setgrent();
	while (p = getgrent()) {
		printf ("%s:%s:%d:", p->gr_name, p->gr_passwd, p->gr_gid);
		i = 0;
		while (p->gr_mem[i]) {
			printf("%s,", p->gr_mem[i]);
			++i;
		}
		putchar('\n');
	}
	endgrent();
}
