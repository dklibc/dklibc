/* Internet scoket address */

#ifndef _ARPA_INET_H
#define _ARPA_INET_H

#define INADDR_ANY		0x00000000
#define INADDR_BROADCAST	0xFFFFFFFF
#define INADDR_NONE		0xFFFFFFFF
#define INADDR_LOOPBACK		0x7f000001

struct in_addr {
	unsigned s_addr;		/* IP addr in network byte order */
};

struct sockaddr_in {
	unsigned short sin_family;	/* AF_INET */
	unsigned short sin_port;	/* port */
	struct in_addr sin_addr;	/* IP */
	char sin_zero[8];		/* reserved */
};

int inet_aton (const char *s, struct in_addr *addr);
char *inet_ntoa (struct in_addr *addr);

#endif
