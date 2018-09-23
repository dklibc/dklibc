#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>

/*
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
*/

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

static int num2hex (unsigned c, char *buf, int min, int add_prefix, int ucase)
{
	char *st, *pref, *s;
	int n;
	
	if (ucase) {
		st = "0123456789ABCDEF";
		n = 'X';
	} else {
		st = "0123456789abcdef";
		n = 'x';
	}

	if (add_prefix) {
		add_prefix = 2;
		*buf++ = '0';
		*buf++ = n;
	}

	if (min)
		memset (buf, '0', min);

	s = buf;
	n = 0;
	do {
		*s++ = st[c & 0xf];
		++n;
		c >>= 4;
	} while (c);

	if (n < min)
		n = min;
	revstr (buf, n);
	return n + add_prefix;
}

static int num2dec (int c, char *buf, int min, int add_plus)
{
	char *s = buf, *q;
	int n;
	
	if (c < 0) {
		*s++ = '-';
		c = -c;
		add_plus = 1;
	} else if (add_plus) {
		*s++ = '+';
		add_plus = 1;
	}

	q = s;

	if (min)
		memset (s, '0', min);

	n = 0;
	do {
		*s++ = '0' + c % 10;
		++n;
		c /= 10;
	} while (c);
	
	if (n < min)
		n = min;

	revstr (q, n);
	return n + add_plus;
}

/*
Try to get positive integer number from string @s. Return pointer to the first
not recognized char and parsed number in @*p. If @*p=0 than it can means that
there is no number or zero
*/
const char *try_num (const char *s, int *p)
{
	int d = 0;
	
	while (isdigit(*s))
		d = d * 10 + *s++ - '0';
	*p = d; 
	return s;
}


#define ALIGN_LEFT 1
#define ALIGN_RIGHT 2

/*
Parse format flags
In: @frmt must point right after '%'
Return: pointer to the first non-recognized char
*width: 0 - no, -1 - arg, >0 - val
*precision: 0 - no, -1 - arg, >0 - val 
*/

/*
Precision:
* max number of decimal digits of float number to print
* min number of digits in integer number to print (padd with zeroes)
* max number of chars from string to print
*/

static const char *parse_flags (const char *frmt, int *width, int *precision, 
int *align, int *padding_char, int *add_plus, int *add_prefix)
{
	int prev;

	*width = 0;
	*precision = 0;
	*align = ALIGN_RIGHT;
	*padding_char = ' ';
	*add_plus = 0;
	*add_prefix = 0;

m1:	switch (*frmt) {
		case '-':
			*align = ALIGN_LEFT;
			++frmt;
			goto m1;
		case '0':
			*padding_char = '0';
			++frmt;
			goto m1;
		case '+':
			*add_plus = 1;
			++frmt;
			goto m1;
		case '#':
			*add_prefix = 1;
			++frmt;
			goto m1;
		case '*':
			++frmt;
			*width = -1;
			if (*frmt != '.')
				break;
		case '.':
m3:			++frmt;
			if (*frmt == '*') {
				*precision = -1;
				++frmt;
			} else
				frmt = try_num (frmt, precision);
			break;
		default:
			frmt = try_num (frmt, width);
			if (!*width)
				break;
			if (*frmt == '.')
				goto m3;
	}
	return frmt;
}

int vsnprintf (char *buf, int bufsz, const char *frmt, va_list ap)
{
	int nleft = bufsz - 1, n, m;
	int align, padding_char, add_prefix, add_plus, field_width, precision;
	char a[32];
	char *s, *q;

	while (*frmt && nleft) {
		if (*frmt != '%' || *++frmt == '%') {
			*buf++ = *frmt++;
			--nleft;
			continue;
		}
				
		frmt = parse_flags (frmt, &field_width, &precision,
		  &align, &padding_char, &add_plus, &add_prefix);

		if (field_width == -1) {
			if ((field_width = va_arg(ap, int)) < 0)
				goto m1;
		}		

		if (precision == -1) {
			if ((precision = va_arg(ap, int)) < 0)
				goto m1;
		}
		
		switch (*frmt++) {
			case 's': /* string */
				n = strlen(s = va_arg (ap, char *));
				if (precision)
					n = n < precision ? n : precision;
				break;
			case 'd': /* signed decimal */
				n = num2dec(va_arg(ap, int), s = a, precision, add_plus);
				break;
			case 'u': /* unsigned decimal */
				goto m1;
			case 'o': /* octal */
				goto m1;
			case 'X': /* hex in upper case */
				n = num2hex(va_arg(ap, unsigned), s = a, precision, add_prefix, 1);
				break;
			case 'x': /* hex in low case */
				n = num2hex(va_arg(ap, unsigned), s = a, precision, add_prefix, 0);
				break;
			case 'c': /* char */
				n = 1;
				a[0] = (char)va_arg(ap, int);
				s = a;
				break;
			case 'p': /* pointer */
				goto m1;
			default:
				goto m1;
		}

		if (field_width > n) {
			m = field_width;
			if (align == ALIGN_LEFT)
				q = buf;
			else
				q = buf + field_width - n;
			if (field_width > nleft)
				goto m1;
			memset (buf, padding_char, field_width);
		} else {
			q = buf;
			m = n;
		}

		if (m > nleft)
			goto m1;

		memcpy (q, s, n);
		buf += m;
		nleft -= m;
	}
m1:	*buf = 0;
	return bufsz - nleft - 1;
}

int snprintf (char *buf, int n, const char *frmt, ...)
{
	va_list ap;
	va_start (ap, frmt);
	return vsnprintf (buf, n, frmt, ap);
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
