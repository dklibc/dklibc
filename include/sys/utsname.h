#ifndef _SYS_UTSNAME_H
#define _SYS_UTSNAME_H

#define _UTSNAME 65

/*
Struct with system info, filled by uname() syscall
NOTE: POSIX doesn't specify the sizes of the fields
NOTE: all strings are terminated with '\0', as usual
*/
struct utsname {
	char sysname[_UTSNAME];		/* System name, e.g. "Linux" */
	char nodename[_UTSNAME];	/* Hostname, e.g. "dksrv" */
	char release[_UTSNAME];		/* Release string, e.g. "2.6.28" */
	char version[_UTSNAME];		/* System version */
	char machine[_UTSNAME];		/* Hardware identifier */
	/* Linux only */
	char domainname[_UTSNAME];	/* Domain name, e.g. "dk-vrn" */
};

#endif

