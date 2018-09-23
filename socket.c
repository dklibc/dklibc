#include <sys/syscall.h>
#include <socketcall.h>
#include <sys/socket.h>

int syscall2 (int sys, ...);

static int socketcall (int call, int *args)
{
	return syscall2 (_SYS_SOCKETCALL, call, args);
}

int socket (int family, int type, int protocol)
{
	return socketcall (_SOCKETCALL_SOCKET, &family);
}

int bind (int fd, const struct sockaddr *addr, int addr_len)
{
	return socketcall (_SOCKETCALL_BIND, &fd);
}

int listen (int fd, int queue_len)
{
	return socketcall (_SOCKETCALL_LISTEN, &fd);
}

int accept (int fd, struct sockaddr *addr, int *addr_len)
{
	return socketcall (_SOCKETCALL_ACCEPT, &fd);
}

int connect (int fd, const struct sockaddr *addr, int addr_len)
{
	return socketcall (_SOCKETCALL_CONNECT, &fd);
}

int recv (int fd, void *buf, int bufsz, int flags)
{
	return socketcall (_SOCKETCALL_RECV, &fd);
}

int send (int fd, const void *buf, int n, int flags)
{
	return socketcall (_SOCKETCALL_SEND, &fd);
}

int recvfrom (int fd, void *buf, int bufsz, int flags, struct sockaddr 
*addr, int *addr_len)
{
	return socketcall (_SOCKETCALL_RECVFROM, &fd);
}

int sendto (int fd, const void *buf, int n, int flags, const struct 
sockaddr *addr, int addr_len)
{
	return socketcall (_SOCKETCALL_SENDTO, &fd);
}

int shutdown (int fd, int how)
{
	return socketcall (_SOCKETCALL_SHUTDOWN, &fd);
}

int setsockopt (int fd, int level, int optname, const void *optval,
int optvalsz)
{
	return socketcall (_SOCKETCALL_SETSOCKOPT, &fd);
}

int getsockopt (int fd, int level, int optname, void *optval, int 
*optvalsz)
{
	return socketcall (_SOCKETCALL_GETSOCKOPT, &fd);
}

int getpeername (int fd, struct sockaddr *addr, int *addr_len)
{
	return socketcall (_SOCKETCALL_GETPEERNAME, &fd);
}

int getsockname (int fd, struct sockaddr *addr, int *addr_len)
{
	return socketcall (_SOCKETCALL_GETSOCKNAME, &fd);
}
