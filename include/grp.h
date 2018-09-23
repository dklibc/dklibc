#ifndef _GRP_H
#define _GRP_H

struct group {
	char *gr_name;
	char *gr_passwd;
	int gr_gid;
	char **gr_mem; /* Group members */
};

struct group *getgrnam (const char *name);
struct group *getgrgid (int gid);

void setgrent(void);
struct group *getgrent(void);
void endgrent(void);

#endif

