/*
gethostname(), getdomainname()
uname() syscall is used in both
*/

#include <unistd.h>
#include <string.h>
#include <errno.h>

/*
Return hostname (e.g. "dksrv") in @buf
*/
int gethostname (char *buf, int sz)
{
	struct utsname u;
	
	if (sz < _UTSNAME) { /* Too small buffer */
		errno = EINVAL;
		return -1;
	}

	uname(&u);
	memcpy (buf, u.nodename, _UTSNAME);
	return 0;
}

/*
Return domain name (e.g. "dk-vrn") in @buf. WARNING: Not POSIX
WARNING: domain name is *not* FQDN!!!
*/
int getdomainname (char *buf, int sz)
{
	struct utsname u;
	
	if (sz < _UTSNAME) { /* Too small buffer */
		errno = EINVAL;
		return -1;
	}

	uname(&u);
	memcpy (buf, u.domainname, _UTSNAME);
	return 0;
}
