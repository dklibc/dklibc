/*
In Linux BSD sockets API in called through one entry point - 
sys_socketcall. You must give it 2 args: arg1 - function number,
arg2 - pointer to array with function args
Here are socket function numbers
*/

#ifndef _SOCKETCALL_H
#define _SOCKETCALL_H

#define _SOCKETCALL_SOCKET	1
#define _SOCKETCALL_BIND	2
#define _SOCKETCALL_CONNECT	3
#define _SOCKETCALL_LISTEN	4
#define _SOCKETCALL_ACCEPT	5
#define _SOCKETCALL_GETSOCKNAME	6
#define _SOCKETCALL_GETPEERNAME	7
#define _SOCKETCALL_SOCKETPAIR	8
#define _SOCKETCALL_SEND	9
#define _SOCKETCALL_RECV	10
#define _SOCKETCALL_SENDTO	11
#define _SOCKETCALL_RECVFROM	12
#define _SOCKETCALL_SHUTDOWN	13
#define _SOCKETCALL_SETSOCKOPT	14
#define _SOCKETCALL_GETSOCKOPT	15
#define _SOCKETCALL_SENDMSG	16
#define _SCOKETCALL_RECVMSG	17

#endif

