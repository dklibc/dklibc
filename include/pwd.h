#ifndef _PWD_H
#define _PWD_H

/* POSIX */
struct passwd {
	char *pw_name;
	char *pw_passwd;
	int pw_uid;
	int pw_gid;
	char *pw_gecos; /* user real name, phone, ... Comma separated list */
	char *pw_dir;
	char *pw_shell;
};

void setpwent(void);
void endpwent(void);
struct passwd *getpwent(void);

struct passwd *getpwnam(const char *uname); /* POSIX */
struct passwd *getpwuid(int uid); /* POSIX */

#endif

