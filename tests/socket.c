#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>

int main (void)
{
	int fd;
	struct sockaddr_in sa;

	fd = socket (AF_INET, SOCK_DGRAM, 0);
	if (fd < 0)
		return -1;

	sa.sin_family = AF_INET;
	sa.sin_port = htons(1503);
	sa.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind (fd, (struct sockaddr *)&sa, sizeof(sa)) < 0)
		return -2;

	while (1);

	return 0;
}
