/* UNIx STandarD syscalls */

#ifndef _UNISTD_H
#define _UNISTD_H

#include <sys/types.h>
#include <sys/utsname.h>

/*
Standard descriptors (stdin, stdout, stderr). POSIX says that portable 
program should use these constants instead of assuming of 0,1,2
*/
#define STDIN_FILENO	0
#define STDOUT_FILENO	1
#define STDERR_FILENO	2

/* Start points for lseek() */
#define SEEK_SET	0
#define SEEK_CUR	1
#define SEEK_END	2

#include <sys/stat.h>

/* File operations */
/*
POSIX defines 2 functions with name open():
1. int open (const char *path, int flags)
2. int open (const char *path, int flags, mode_t mode)
You must specify @mode when flag O_CREAT is used.
But in C (unlike C++) there can't be several functions with the same 
name, so, to satisfy this we must use variable length arglist func
*/
int open (const char *path, int flags, ...);
int close (int fd);
/* Equals to open (path, O_WRONLY|O_CREAT|O_TRUNC, mode) */
int creat (const char *path, int mode);
int write (int fd, const void *buf, int sz);
int read (int fd, void *buf, int sz);
int lseek (int fd, int off, int from);
int rename (const char *oldpath, const char *newpath);
int truncate (const char *path, int to_len);
int ftruncate (int fd, int to_len);
int link (const char *old, const char *new);
int unlink (const char *path);
int mkdir (const char *path, int mode);
int rmdir (const char *path);
int stat (const char *fname, struct stat *p);

/* Change process current working directory */
int chdir (const char *path);

/* Get current working directory of process */
char *getcwd (char *buf, int sz);

/* After at min @seconds seconds you will get SIGALRM signal */
int alarm (unsigned seconds);

/* Pause until any signal */
int pause (void);

/* Exec new process */
int fork (void);
int execve (const char *fname, char *argv[], char *envp[]);
int execv (const char *fname, char *argv[]);
int execl (const char *fname, char *arg, ...);

/* Send signal to process */
int kill (int pid, int sig);

/* Get current Unix time */
time_t time (time_t *t);

/* Duplicate descriptors */
int dup (int oldfd);
int dup2 (int oldfd, int newfd);

/* Create pipe */
int pipe (int pipefd[2]);

/* Flush all buffers and inodes to disks */
void sync (void);

/* Exit process (prefixed with '_' to not to conflict with libc exit()) */
void _exit (int status);

/* Wait child process to terminate, reap zombies  */
int wait (int *status);
int waitpid (int pid, int *status, int options);

/* Get process PID */
int getpid(void);
/* Get process parent PID */
int getppid(void);

/*
Create a new session, first process group in it and become a session 
leader of this new session
*/
int setsid(void);

/*
Not included in POSIX, but are legacy Unix syscalls. Shift process 
"break boundary" (used by heap allocation functions like malloc)
*/
int brk (void *addr);
void *sbrk (int inc);

/* Return: !=0 - if @fd points to tty, 0 - otherwise */
int isatty (int fd);

/* Device I/O control */
int ioctl (int fd, int cmd, void *arg);

/* Parse program options */
extern char *optarg;
extern int optind, opterr, optopt;
int getopt (int argc, char *argv[], const char *optstring);

/*
Read password from the process controlling terminal (/dev/tty)
Not POSIX, but SUSv2 and BSD and glibc
*/
char *getpass(const char *promt);

/* Change file access mode. Only root can do this */
int chmod(const char *fname, int mode);
int lchmod(const char *fname, int mode);
int fchmod(int fd, int mode);

/* Change owner and group of file. Only root can do this */
int chown(const char *fname, int uid, int gid);

/* Get UID, GID. Always successful */
int getuid(void);
int getgid(void);

int gethostname (char *buf, int sz);
int getdomainname (char *buf, int sz); /* WARNING: not POSIX */
int setdomainname (const char *s, int n); /* WARNING: not POSIX */
int sethostname (const char *s, int n); /* WARNING: not POSIX */

/* Fill in system info in @p, uname - Unix Name */
int uname (struct utsname *p);

unsigned sleep (unsigned sec);

int pause (void);

int mknod (const char *fname, int mode, int dev);

int umask (int mask);

#endif

