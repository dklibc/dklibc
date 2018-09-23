#include <unistd.h>

int main (int argc, char *argv[])
{
	char buf[32];
	int n;
	
	write (1, "Input your name: ", 17);
	n = read (0, buf, 32);
	write (1, "Hello, ", 7);
	write (1, buf, n);
	return 0;
}
