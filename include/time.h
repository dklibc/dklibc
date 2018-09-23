#ifndef _TIME_H
#define _TIME_H

#ifndef _SYS_TYPES_H
typedef int time_t;
#endif

struct tm {
	int tm_sec;	// 0-61
	int tm_min;	// 0-59
	int tm_hour;	// 0-23
	int tm_mday;	// Day of month (1-31)
	int tm_mon;	// Month _since_ Jan (0-11)
	int tm_year;	// Year _since_ 1900
	int tm_wday;	// days since Sunday (0-6)
	int tm_yday;	// days since Jan 1 (0-365)
	int tm_isdst;	// Daylight Saving Time flag
};

time_t time (time_t *ptime);
time_t mktime (struct tm *p);
int strftime (char *buf, int sz, const char *frmt, struct tm *p);
struct tm *gmtime (const time_t *ptime);
struct tm *localtime (const time_t *ptime);
int stime (time_t *pt); /* Set system time. Missed in POSIX and libc */
char *asctime(struct tm *p);
char *ctime(time_t *t);

#endif
