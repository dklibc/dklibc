/*
POSIX time:
* POSIX time - seconds elapsed since Epoch - 1 Jan 1970 00:00:00 GMT
* Each and every day is exactly 86400 seconds (no leap seconds)
* Unix'es on 32-bit machines define time_t as int (32-bit signed integer)
* 32-bit signed int time will overflow in 2038, so modern Unix'es uses 
64-bit signed integer for time
* Negative time can address time before Epoch (but this doesn't used?)
*/

/*
Leap year rule in Georgian Calendar: year must be divisible by 4, if 
divisible by 100 must be divisible by 400 too. Leap year has additional 
day (366 vs 365)
*/

/*
WARNING: no local time support yet
*/

/*
Your RTC clock must be set to UTC time since Linux gets base time 
from here while booting. Or in start script you can run hwclock to 
fix system time from local RTC to UTC
*/

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <errno.h>

static struct tm tm;
static int tz_hours = 3; /* Local time = UTC + tz_hours */

static int isleap (int y)
{
	if (y % 4)
		return 0;

	// NOTE: Since 1970 till 2038 no years that are rejected by this rule
	if (!(y % 100) && (y % 400))
		return 0;

	return 1;
}

static int mday (int m, int leap_year)
{
	static char d0[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30,
				 31, 30, 31 };
	static char d1[12] = { 31, 29, 31, 30, 31, 30, 31, 31, 30,
				 31, 30, 31 };
	return leap_year ? d1[m] : d0[m];
}

static int normalize_tm (struct tm *p)
{
	int i;
	
	/*
	WRONG: they *can* be negative!!! To make it possible for user to 
	do date arithmetic like: "substract from this date 3 months"
	*/
	if (p->tm_sec < 0 || p->tm_min < 0 || p->tm_hour < 0 ||
	    p->tm_mday < 0 || p->tm_mon < 0 || p->tm_year < 70)
		return -1;

	/*
	Normalize in order: tm_sec, tm_min, tm_hour, tm_mday, tm_mon, 
	tm_year
	*/
	
	if (p->tm_sec > 59) {
		p->tm_sec -= 60;
		++(p->tm_min);
	}

	if (p->tm_min > 59) {
		p->tm_min -= 60;
		++(p->tm_hour);
	}

	if (p->tm_hour > 23) {
		p->tm_hour -= 24;
		++(p->tm_mday);
	}

	if (p->tm_mon > 11) {
		p->tm_mon -= 12;
		++(p->tm_year);
	}

	if (p->tm_mday > (i = mday(p->tm_mon, isleap(p->tm_year + 1900)))) {
		p->tm_mday -= i;
		++(p->tm_mon);
	}

	if (p->tm_mon > 11) {
		p->tm_mon = 1;
		++(p->tm_year);
	}

	// Compute @tm_yday
	for (i = 0, p->tm_yday = 0; i < p->tm_mon; ++i)
		p->tm_yday += mday(i, p->tm_year);
	p->tm_yday += p->tm_mday - 1;

	// TODO: Compute @tm_wday
	return 0;
}

/*
@p - represents local time
We must make Unix time from local time
* Values of @tm_yday and @tm_wday is ignored and will be computed from 
values of the rest fields and stored there
* If values of the other fields are outside of its valid ranges - 
they will be normalized
* In: @isdst: >0 - Daylight Saving Time is applied for the specified 
time, =0 -not applied,  <0 - guess byself from timezone info. Out: must 
be set from timezone info
*/
int mktime (struct tm *p)
{	
	errno = EINVAL;
	if (normalize_tm(p) < 0)
		return -1;

	return p->tm_sec + p->tm_min * 60 + p->tm_hour * 3600 + 
p->tm_yday * 86400 + (p->tm_year - 70) * 31536000 + ((p->tm_year - 
69)/4) * 86400; /* Every 4th year additional day is added (leap day - 
leap year) Since 1970 till 2038 there is a single year - 2000 that 
divisible by 100, but it also divisible by 400 */
}

#define YEARSZ(y) (isleap(y) ? 366 : 365)

#define SECONDS_IN_DAY 86400

struct tm *gmtime (const time_t *ptime)
{
	unsigned s, d;
	int t = *ptime, i, y, m;

	s = (unsigned) t % SECONDS_IN_DAY; // Seconds since 00:00:00
	d = (unsigned) t / SECONDS_IN_DAY; // Days since Epoch
	
	tm.tm_sec = s % 60;
	tm.tm_min = (s % 3600) / 60;
	tm.tm_hour = s / 3600;
	
	tm.tm_wday = (d + 4) % 7; /* day 0 was a thursday */
	
	y = 1970;
	while (d > (i = YEARSZ(y))) {
		d -= i;
		++y;
	}
	tm.tm_year = y - 1900; // Years since 1900
	tm.tm_yday = d; // Days since year beginning

	m = 0;
	s = isleap(1900 + y);
	while (d > (i = mday(m, s))) {
		d -= i;
		++m;
	}
	tm.tm_mon = m;
	tm.tm_mday = d + 1;
	
	tm.tm_isdst = 0;
	return &tm;
}

struct tm *localtime (const time_t *ptime)
{
	time_t t;

	t = *ptime + tz_hours * 3600;
	return gmtime (&t);
}

static char *xy2str (char *buf, int n)
{
	*buf = '0' + n / 10;
	*(buf+1) = '0' + n % 10;
	return buf + 2;
}

int strftime (char *buf, int bufsz, const char *frmt, struct tm *p)
{
	static char *mon[12] = { "Jan", "Feb", "Mar", "Apr", "May",
		"Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
	static char *wday[7] = { "Sun", "Mon", "Tue", "Wed", "Thu", 
		"Fri", "Sat" };
	int nleft = bufsz - 1;

	while (nleft && *frmt) {
		if (*frmt != '%') {
			*buf++ = *frmt++;
			--nleft;
		} else {
			switch (*++frmt) {
				case 'a': /* Abbreviated weekday name */
			if (nleft < 3 || p->tm_wday < 0 || p->tm_wday > 6) {
				nleft = bufsz;
				goto m1;
			}
			memcpy (buf, wday[p->tm_wday], 3);
			buf += 3;
			nleft -= 3;
					break;
				case 'b': /* Abbreviated month name */
			if (nleft < 3 || p->tm_mon < 0 || p->tm_mon > 11) {
				nleft = bufsz;
				goto m1;
			}
			memcpy (buf, mon[p->tm_mon], 3);
			buf += 3;
			nleft -= 3;
					break;
				case 'd': /* Day of the month (01-31) */
			if (nleft < 2 || p->tm_mday < 1 || p->tm_mday > 31) {
				nleft = bufsz;
				goto m1;
			}
			buf = xy2str(buf, p->tm_mday);
			if (*(buf - 2) == '0')
				*(buf - 2) = ' ';
			nleft -= 2;
					break;
				case 'H': /* Hour (00-23) */
			if (nleft < 2 || p->tm_hour < 0 || p->tm_hour > 23) {
				nleft = bufsz;
				goto m1;
			}
			buf = xy2str(buf, p->tm_hour);
			nleft -= 2;
					break;
				case 'm': /* Month (01-12) */
			if (nleft < 2 || p->tm_mon < 0 || p->tm_mon > 11) {
				nleft = bufsz;
				goto m1;
			}
			buf = xy2str(buf, p->tm_mon + 1);
			nleft -= 2;
					break;
				case 'M': /* Minute (00-59) */
			if (nleft < 2 || p->tm_min < 0 || p->tm_min > 59) {
				nleft = bufsz;
				goto m1;
			}
			buf = xy2str(buf, p->tm_min);
			nleft -= 2;
					break;
				case 'S': /* Second (00-59) */
			if (nleft < 2 || p->tm_sec < 0 || p->tm_sec > 59) {
				nleft = bufsz;
				goto m1;
			}
			buf = xy2str(buf, p->tm_sec);
			nleft -= 2;
					break;
				case 'y': /* Year without century */
			if (nleft < 2 || p->tm_year < 70) {
				nleft = bufsz;
				goto m1;
			}
			buf = xy2str(buf, (1900 + p->tm_year) % 100);
			nleft -= 2;
					break;
				case 'Y': /* Year with century */
			if (nleft < 4 || p->tm_year < 70) {
				nleft = bufsz;
				goto m1;
			}
			sprintf (buf, "%d", p->tm_year + 1900);
			buf += 4;
			nleft -= 4;
					break;
				case 'z': /* Timezone +/-hhmm */
			if (nleft < 5) {
				nleft = bufsz;
				goto m1;
			}
			memcpy(buf, "+0000", 5);
			buf += 5;
			nleft -= 5; 
					break;
				case '%':
					*buf++ = '%';
					break;
				default:
					goto m1;
			}
			++frmt;
		}
	}
m1:	*buf = '\0';
	return bufsz - nleft;
}

/* ascii time */
char *asctime (struct tm *p)
{
	static char buf[64];
	
	strftime (buf, 64, "%a %b %d %H:%M:%S %Y\n", p);
	return buf;
}

char *ctime (time_t *t)
{
	return asctime (localtime(t));
}
