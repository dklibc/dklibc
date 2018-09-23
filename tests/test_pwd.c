#include <stdio.h>
#include <pwd.h>

void main (int argc, char *argv[])
{
	struct passwd *p;
	
	p = getpwnam(argv[1]);
	if (!p) {
		puts ("Not found");
		return;
	}
	printf ("%s %s %d %d %s %s\n", p->pw_name, p->pw_passwd, 
p->pw_uid, p->pw_gid, p->pw_dir, p->pw_shell);
}
