/* Linux sycall numbers */

#ifndef _SYS_SYSCALL_H
#define _SYS_SYSCALL_H

#define _SYS_EXIT	1
#define _SYS_FORK	2
#define _SYS_READ	3
#define _SYS_WRITE	4
#define _SYS_OPEN	5
#define _SYS_CLOSE	6
#define _SYS_WAITPID	7
#define _SYS_CREAT	8
#define _SYS_LINK	9
#define _SYS_UNLINK	0xa
#define _SYS_EXECVE	0xb
#define _SYS_CHDIR	0xc
#define _SYS_TIME	0xd
#define _SYS_MKNOD	0xe
#define _SYS_CHMOD	0xf
#define _SYS_LSEEK	0x13
#define _SYS_GETPID	0x14
#define _SYS_MOUNT	0x15
#define _SYS_UMOUNT	0x16
#define _SYS_STIME	0x19
#define _SYS_ALARM	0x1b
#define _SYS_PAUSE	0x1d
#define _SYS_UTIME	0x1e
#define _SYS_SYNC	0x24
#define _SYS_KILL	0x25
#define _SYS_RENAME 	0x26
#define _SYS_MKDIR	0x27
#define _SYS_RMDIR	0x28
#define _SYS_DUP	0x29
#define _SYS_PIPE	0x2a
#define _SYS_BRK	0x2d
#define _SYS_SIGNAL	0x30
#define _SYS_IOCTL	0x36
#define _SYS_FCNTL	0x37
#define _SYS_UMASK	0x3c
#define _SYS_DUP2	0x3f
#define _SYS_SETSID	0x42
#define _SYS_SIGACTION	0x43
#define _SYS_SETHOSTNAME 0x4a
#define _SYS_GETTIMEOFDAY 0x4e
#define _SYS_SETTIMEOFDAY 0x4f
#define _SYS_SWAPON	0x57
#define _SYS_REBOOT	0x58
#define _SYS_READDIR	0x59
#define _SYS_TRUNCATE	0x5c
#define _SYS_FTRUNCATE	0x5d
#define _SYS_FCHMOD	0x5e
#define _SYS_SOCKETCALL	0x66 /* BSD sockets interface entrypoint */
#define _SYS_STAT	0x6a
/* #define _SYS_UNAME	0x6d */
#define _SYS_SWAPOFF	0x73
#define _SYS_SETDOMAINNAME	0x79
#define _SYS_UNAME	0x7a
#define _SYS_SIGPROCMASK	0x7e
#define _SYS_SELECT	0x8e
#define _SYS_GETCWD	0xb7
#define _SYS_STAT64	0xc3
#define _SYS_LCHOWN	0xc6
#define _SYS_GETUID	0xc7
#define _SYS_GETGID	0xc8
#define _SYS_GETEUID	0xc9
#define _SYS_GETEGID	0xca
#define _SYS_FCHOWN	0xcf
#define _SYS_CHOWN	0xd4
#define _SYS_SETUID	0xd5
#define _SYS_SETGID	0xd6
#define _SYS_GETDENTS	0x8d

#endif

