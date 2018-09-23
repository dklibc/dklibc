#include <stdio.h>
#include <stdarg.h>
#include <string.h>

static int cpystr (char *to, char *from, int max)
{
	int n = 0;

	while (n < max) {
		if (!(*to++ = *from++))
			break;
		++n;
	}
	return n;
}

static void revstr (char *s, int n)
{
	char *p = s, *q = s + n - 1;
	int c;

	do {
		c = *p;
		*p++ = *q;
		*q-- = c;
	} while (p < q);
}

static int num2hex (unsigned c, char *buf)
{
	static char *t = "0123456789ABCDEF";
	int n = 0;
	char *s = buf;
	
	do {
		*s++ = t[c & 0xf];
		++n;
		c >>= 4;
	} while (c);
	*s = 0;
	revstr (buf, n);
	return n;
}

static int num2dec (int c, char *buf)
{
	char *s = buf;
	int n = 0;
	
	if (c < 0) {
		*s++ = '-';
		c = -c;
	}

	do {
		*s++ = '0' + c % 10;
		++n;
		c /= 10;
	} while (c);
	*s = 0;
	revstr (buf, n);
	return n;
}

int vsnprintf (char *buf, int bufsz, const char *frmt, va_list ap)
{
	int nleft = bufsz - 1, m;
	char a[16];
	
	while (*frmt && nleft) {
		if (*frmt == '%') {
			switch (*++frmt) {
				case 's':
		m = cpystr(buf, va_arg (ap, char *), nleft);
		buf += m;
		nleft -= m;
		break;
				case 'd':
		m = num2dec(va_arg(ap, int), a);
		goto m3;
					break;
				case 'x':
		m = num2hex(va_arg(ap, unsigned), a);
m3:		m = m < nleft ? m : nleft;
		memcpy(buf, a, m);
		buf += m;
		nleft -= m;
					break;
				case 'c':
		*buf++ = (char)va_arg(ap, int);
		--nleft;
				break;
				default:
					goto m1;
			}
			++frmt;
		} else {
			*buf++ = *frmt++;
			--nleft;
		}
	}
m1:	*buf = 0;
	return bufsz - nleft - 1;
}

int sprintf (char *buf, const char *frmt, ...)
{
	va_list ap;
	va_start (ap, frmt);
	return vsnprintf (buf, 4096, frmt, ap);
}

int vfprintf (FILE *fp, const char *frmt, va_list ap)
{
	char buf[4096];
	int n;
	
	n = vsnprintf (buf, 4096, frmt, ap);
	if (fwrite (buf, n, 1, fp) < n)
		return -1;
	return n;
}

int fprintf (FILE *fp, const char *frmt, ...)
{
	va_list ap;
	va_start(ap, frmt);
	return vfprintf (fp, frmt, ap);
}

int printf (const char *frmt, ...)
{
	va_list ap;
	va_start(ap, frmt);
	return vfprintf (stdout, frmt, ap);
}
