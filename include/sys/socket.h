/* Main header for BSD socket functions */

#ifndef _SYS_SOCKET_H
#define _SYS_SOCKET_H

#include <arpa/inet.h>	/* Internet socket address */
#include <sys/un.h>	/* Unix domain socket address */
#include <netinet/in.h> /* htons, htonl, ... */

/* Generic socket addr struct */
struct sockaddr {
	unsigned short sa_family;
	char sa_data[14];
};

// Socket types
#define SOCK_STREAM	1
#define SOCK_DGRAM	2
#define SOCK_RAW	3

// Socket Level (second arg of setsockopt(), getsockopt())
#define SOL_SOCKET	1

// Socket options (see setsockopt(), getsockopt())
#define SO_REUSEADDR	2
#define SO_LINGER	13
#define SO_RCVTIMEO	20

// Protocol families (first arg of socket())
#define PF_LOCAL	1
#define PF_UNIX		PF_LOCAL
#define PF_INET		2

// Address families - the same as protocol families
#define AF_INET		PF_INET
#define AF_UNIX		PF_LOCAL
#define AF_LOCAL	PF_LOCAL

// shutdown() second arg
#define SHUT_RD		0
#define SHUT_WR		1
#define SHUT_RDWR	2

int socket (int family, int type, int protocol);
int bind (int fd, const struct sockaddr *addr, int addr_len);
int listen (int fd, int queue_len);
int accept (int fd, struct sockaddr *addr, int *addr_len);
int connect (int fd, const struct sockaddr *addr, int addr_len);
int recv (int fd, void *buf, int n, int flags);
int send (int fd, const void *buf, int n, int flags);
int recvfrom (int fd, void *buf, int n, int flags, struct 
  sockaddr *addr, int *addr_len);
int sendto (int fd, const void *buf, int n, int flags, const 
  struct sockaddr *addr, int addr_len);
int getpeername (int fd, struct sockaddr *addr, int *addr_len);
int getsockname (int fd, struct sockaddr *addr, int *addr_len);
int shutdown (int fd, int how);
int getsockopt (int fd, int level, int optname, void *optval, int *optsz);
int setsockopt (int fd, int level, int optname, const void *optval, int optsz);

#endif

