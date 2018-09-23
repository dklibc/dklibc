#ifndef _UTMP_H
#define _UTMP_H

#include <sys/types.h>
#include <unistd.h>

// Record types
#define EMPTY		0
#define BOOT_TIME	2 /* System boot time record */
#define INIT_PROCESS	5
#define LOGIN_PROCESS	6
#define USER_PROCESS	7
#define DEAD_PROCESS	8

#define UT_LINESZ	32
#define UT_NAMESZ	32
#define UT_HOSTSZ	256

// Lastlog entry
struct lastlog {
	time_t ll_time;
	char ll_line[UT_LINESZ];
	char ll_host[UT_HOSTSZ]; 
};

// UTMP and WTMP entry
struct utmp {
	short ut_type;		// Record type
	int ut_pid;		// PID of login process
	char ut_line[UT_LINESZ]; // Login terminal name without "/dev"
	char ut_id[4];		// Terminal name suffix or inittab ID
	char ut_user[UT_NAMESZ]; // User login name
	char ut_host[UT_HOSTSZ]; // Name of the host the user logged from
	int reserved;
	int ut_session;		/* Sesssion ID */
	struct {
		int sec;
		int usec;
	} ut_tv;	/* Time the entry was made */
	unsigned ut_addr_v6[4];	// IPv6 address of the host the user logged from
				// IPv4 is just ut_addr_v6[0]
	char __reserved[20];
};

struct utmp *getutent(void);
void setutent(void);
void endutent(void);
struct utmp *getutid(struct utmp *);
struct utmp *getutline(struct utmp *);
struct utmp *pututline(struct utmp *);

#endif

