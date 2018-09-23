/* Functions to read/search /etc/passwd */

#include <pwd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINE_SZ 128
#define NFIELDS 7

static struct passwd pwent;
static char buf[MAX_LINE_SZ];
static FILE *fp;

/* Parses passwd entry. Return: 0 - OK, -1 - error */
static int parse_pwd_line (void)
{
	char *p, *s;
	char *w[NFIELDS];
	int i, n;

	i = 0;
	s = buf;
	while (i < NFIELDS && s) {
		w[i++] = s;
		p = strchr (s, ':');
		if (p)
			*p++ = '\0';
		s = p;
	}
	
	if (s)
		return -1; /* Too many fields */
	
	while (i < NFIELDS)
		w[i++] = "";

	pwent.pw_name = w[0];
	pwent.pw_passwd = w[1];
	if ((pwent.pw_uid = __str2num(w[2])) < 0)
		return -1;
	if ((pwent.pw_gid = __str2num(w[3])) < 0)
		return -1;
	pwent.pw_gecos = w[4];
	pwent.pw_dir = w[5];
	pwent.pw_shell = w[6];
	
	return 0;
}

/* Open passwd file or rewind it to start */
void setpwent (void)
{
	if (fp) {
		if (fseek (fp, 0, SEEK_SET) < 0) {
			fclose (fp);
			fp = NULL;
		}
		return;
	}
	
	fp = fopen ("/etc/passwd", "r");
}

/* Close passwd file */
void endpwent (void)
{
	fclose (fp);
	fp = NULL;
}

/*
Get next entry. Return NULL in case of EOF or error. So empty line or 
invalid record will be treated by caller as EOF
*/
struct passwd *getpwent (void)
{
	if (__getline(buf, MAX_LINE_SZ, fp) <= 0)
		return NULL;
	
	if (parse_pwd_line () < 0)
		return NULL;

	return &pwent;
}

/*
Get passwd entry by user name. Return NULL in case of not found or 
error. Used by login. POSIX API
*/
struct passwd *getpwnam (const char *uname)
{
	struct passwd *p;
	
	setpwent();
	
	/* Not fast, but simple... */
	while ((p = getpwent()) && strcmp(p->pw_name, uname));

	endpwent();
	return p;
}

/*
Get passwd entry by user ID. Return NULL in case of not found or 
error. Used by any program that whats to get user name by its ID. 
POSIX API!
*/
struct passwd *getpwuid (int uid)
{
	struct passwd *p;

	setpwent();

	/* Not fast, but simple... */
	while ((p = getpwent()) && (p->pw_uid != uid));

	endpwent();
	return p;
}

