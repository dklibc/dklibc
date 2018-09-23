/*
I need my own version of fgets() since original fgets() usually doesn't fit my
needs: to determine "too-long-line" error you need examine last char of the
read line - if it is '\n'? But fgets() doesn't return line length. So, you
need call strlen(). And you need to remove '\n' from end of line in buffer.
*/

#include <stdio.h>

/*
Read line from file. Line separator (NL) isn't stored in buffer.
Last in buffer is stored 0. When EOF is encountered before NL
and some chars have been already read then return number of chars read
and only at next call return "No lines left".

Return: >0 - line length + 1, 0 - no lines left, <0 - error code 
(-1 - read error, -2 - too long line)
*/

#define NL 10
#define CR 13

int __getline (char *buf, int bufsz, FILE *fp)
{
	int c, i;

	i = 0;
	while ((c = fgetc(fp)) != NL) {
		if (c == EOF) {
			if (ferror(fp))
				return -1; /* Read error */
			else
				if (i)
					break;
				else
					return 0; /* No lines left */
		}
		if (i == bufsz - 1)
			return -2; /* Too long line */
		buf[i++] = c;
	}

	/* Remove CR from the end of line */
	if (i && buf[i - 1] == CR)
		--i;

	buf[i] = 0;
	return i + 1; /* line length + 1 */
}
