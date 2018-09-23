#ifndef _SHADOW_H
#define _SHADOW_H

/* Entry of /etc/shadow */
struct spwd {
	char *sp_namp; /* User name */
	char *sp_pwdp; /* Encrypted password */
	
	/* Password aging features */
	/* Date of the last pwd change in days since Epoch. 0 - means 
user is to be asked to change pwd at the next login . Empty - password 
aging features are disabled */
	int sp_lstchg;
	int sp_min;	/* Min pwd age in days. Can be 0 or empty */
	int sp_max;	/* Max pwd age in days. Can be empty. If 
max < min then user can't change his pwd at all */
	/* Num of days before pwd expires to warn user to change it */
	int sp_warn;
	/* Num of days after pwd expires until user can log on by the expired
	pwd */
	int sp_inact;
	
	/* Date when *account* expires in days since Epoch. Empty field means -
	"never expired" */
	int sp_expire;
	unsigned sp_flag; /* reserved, must be empty? */
};

void sentspent(void);
void endspent(void);
struct spwd *getspent(void);
struct spwd *getspnam(const char *name);

#endif

