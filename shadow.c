/* Functions to read /etc/shadow file */

/*
/etc/shadow - passwd DB, not even *readable* (and writable of course!) 
by ordinal users (not root). There encrypted passwords are stored.

Each line has 9 fields separated by commas whith password and optional 
pwd aging info: 

* char[] user_name

* char[] encrypted_pwd - can also be empty (in this case user hasn't 
pwd) and '*' (user is disallowed to login with password)

* int pwd_last_change_date: date of the last pwd change. 0 - means user 
will be warned and offered to change his pwd at the next login. Empty - 
no pwd aging features are applied

* int pwd_min_age: min number of days since pwd change when user is 
NOT allowed to change his pwd. Can be empty and 0

* int pwd_max_age: max number of days till pwd can be valid. Can be 
empty. If max < min then user isn't allowed to change his pwd

* int pwd_warn_days: number of days before pwd expiration date when user is
warned and offered to change his password. Can be 0 and empty

* int pwd_inact_days: number of days since pwd expiration date when user 
can login by his expired password

* int account_expiration_date: date of account expiration in days since Epoch
Empty means endless account
*/

#include <shadow.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_SZ 128
#define NFIELDS 9

static struct spwd spwent;
static char buf[MAX_LINE_SZ];
static FILE *fp;

/* Numeric field can be empty or >= 0 */
static int do_num_field (const char *s, int *p)
{
	if (!*s) {
		*p = -1; /* Empty field */
		return 0;
	}
	return *p = __str2num(s);
}

/*
Parse shadow entry. Return: 0 - OK, -1 - error
If numeric field is empty then set it to -1
*/
static int parse_spwd_line (void)
{
	char *p, *s;
	char *w[NFIELDS];
	int i;

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

	spwent.sp_namp = w[0];
	spwent.sp_pwdp = w[1];	
	
	if (do_num_field(w[2], &spwent.sp_lstchg) < 0)
		return -1;

	if (spwent.sp_lstchg == -1) { /* No pwd aging features */
		spwent.sp_min = spwent.sp_max = spwent.sp_warn =
		  spwent.sp_inact = 0;	
	} else { /* There are pwd aging features */
		if (do_num_field(w[3], &spwent.sp_min) < 0)
			return -1;

		if (do_num_field(w[4], &spwent.sp_max) < 0)
			return -1;

		if (do_num_field(w[5], &spwent.sp_warn) < 0)
			return -1;

		if (do_num_field(w[6], &spwent.sp_inact) < 0)
			return -1;
	}
	
	if (do_num_field(w[7], &spwent.sp_expire) < 0)
		return -1;
	return 0;
}

/*
Open shadow file or rewind position to start
*/
void setspent (void)
{
	if (fp) {
		if (fseek (fp, 0, SEEK_SET) < 0) {
			fclose (fp);
			fp = NULL;
		}
		return;
	}
	
	fp = fopen ("/root/denix/shadow", "r");
}

/*
Close shadow file
*/
void endspent (void)
{
	fclose (fp);
	fp = NULL;
}

/*
Get next entry. Return NULL in case of not found or error. So empty line 
and invalid entry caller probably will be treat as EOF
*/
struct spwd *getspent (void)
{
	if (__getline(buf, MAX_LINE_SZ, fp) <= 0)
		return NULL;
	
	if (parse_spwd_line () < 0)
		return NULL;

	return &spwent;
}

/*
Find entry by user name. Return NULL in case of not found or error
Used by login
*/
struct spwd *getspnam (const char *uname)
{
	struct spwd *p;
	char *s;
	
	setspent();
	
	/* Not fast, but simple... */
	while ((p = getspent()) && strcmp(p->sp_namp, uname));

	endspent();
	return p;
}

