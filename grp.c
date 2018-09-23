/* Functions to read/search /etc/group */

/*
/etc/group - text file, groups DB, each line - entry of 4 fields 
separated with ':' with info about one group. This file must be 
writable only by root and readable by everyone to be able to match GID 
to group name

Entry format: group_name:group_password:gid:comma_separated_list_of_members
Usually group_password is empty
*/

#include <grp.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINE_SZ 128
#define NFIELDS 4
#define MAX_GRP_SZ 8 /* Max 8 users in group */

static struct group grpent;
static char buf[MAX_LINE_SZ];
static char *grpmem[MAX_GRP_SZ+1];
static FILE *fp;

/*
Parse group entry in @buf to @grpent. Return: 0 - OK, -1 - error
*/
static int parse_grp_line (void)
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

	grpent.gr_name = w[0];
	grpent.gr_passwd = w[1];
	if ((grpent.gr_gid = __str2num(w[2])) < 0)
		return -1;
	grpent.gr_mem = grpmem;
	
	s = w[3];
	i = 0;
	while (i < MAX_GRP_SZ && s) {
		grpmem[i++] = s;
		p = strchr(s, ',');
		if (p)
			*p++ = '\0';
		s = p;
	}
	
	if (s)
		return -1; /* Too many users in group */

	if (!grpmem[i-1][0])
		--i;
	grpmem[i] = (char *)0;

	return 0;
}

/*
Open group file or rewind it to start
*/
void setgrent (void)
{
	if (fp) {
		if (fseek (fp, 0, SEEK_SET) < 0) {
			fclose (fp);
			fp = NULL;
		}
		return;
	}
	
	fp = fopen ("/etc/group", "r");
}

/*
Close group file
*/
void endgrent (void)
{
	fclose (fp);
	fp = NULL;
}

/*
Get next entry. Return NULL in case of EOF or error. So, empty line or 
invalid entry will be treated by caller as EOF
*/
struct group *getgrent (void)
{
	if (__getline(buf, MAX_LINE_SZ, fp) <= 0)
		return NULL;
	
	if (parse_grp_line () < 0)
		return NULL;

	return &grpent;
}

/*
Search in group file by group name. Return NULL in case of not found or 
error. POSIX API!
*/
struct group *getgrnam (const char *gname)
{
	struct group *p;
	
	setgrent();
	
	/* Not fast, but simple... */
	while ((p = getgrent()) && strcmp (p->gr_name, gname));

	endgrent();
	return p;
}

/*
Search in group file by GID. Return NULL in case of not found or error.
Used by programs that need to get group name and the rest info by GID
POSIX API!
*/
struct group *getgrgid (int gid)
{
	struct group *p;

	setgrent();

	/* Not fast, but simple... */
	while ((p = getgrent()) && (p->gr_gid != gid));

	endgrent();
	return p;
}

