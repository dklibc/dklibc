////////////// Linux syscalls wrappers ///////////////////

#include <sys/syscall.h>
#include <errno.h>
#include <sys/utsname.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <utime.h>
#include <sys/mount.h>

int syscall0 (int n);
int syscall1 (int n, ...);
int syscall2 (int n, ...);
int syscall3 (int n, ...);
int syscall4 (int n, ...);
int syscall5 (int n, ...);

extern void *_heap_beg;
extern void *_heap_end;
extern int errno;

/*
If O_CREAT flag is set you *must* specify the third arg - int @mode. 
Otherwise it will be ignored. The effective mode is modified by 
process's umask in the usual way: mode & ~umask
*/
int open (const char *path, int flags, ...)
{
	int mode = 0;
	va_list v;

	if (flags | O_CREAT) {
		va_start (v, flags);
		mode = va_arg (v, int);
	}
		
	return syscall3 (_SYS_OPEN, path, flags, mode);
}

/*
creat() is equiv to open() with flags equal to O_CREAT|O_WRONLY|O_TRUNC
*/
int creat (const char *path, int mode)
{
	return syscall2 (_SYS_CREAT, path, mode);
}

int close (int fd)
{
	return syscall1 (_SYS_CLOSE, fd);
}

int write (int fd, const void *buf, int sz)
{
	return syscall3 (_SYS_WRITE, fd, buf, sz);
}

int read (int fd, void *buf, int sz)
{
	return syscall3 (_SYS_READ, fd, buf, sz);
}

int lseek (int fd, int off, int from)
{
	return syscall3 (_SYS_LSEEK, fd, off, from);
}

/*
Return: To parent process child's PID in case of OK, -1 in case of 
error. To child process return 0. "copy-on-write" pages method
*/
int fork (void)
{
	return syscall1 (_SYS_FORK, 0);
}

/*
Executes program @fname. @fname must be either a binary executable, or
a script starting with line: "#!interpreter [optional args]"
@argv[] - array of pointers to args, first should point to program 
filename, last must be NULL
@envp[] - array of pointers to name=val pairs, last must be NULL
Doesn't return on success< and the text, data, bss, and stack of the
calling process are overwritten by that of the program loaded
*/
int execve (const char *fname, char *argv[], char *envp[])
{
	return syscall3 (_SYS_EXECVE, fname, argv, envp);
}

extern char **environ;

int execv (const char *fname, char *argv[])
{
	return syscall3 (_SYS_EXECVE, fname, argv, environ);
}

int execl (const char *fname, char *arg, ...)
{
	return syscall3 (_SYS_EXECVE, fname, &arg, environ);
}

/*
Change data segment size (location of program break, which defines 
program heap end). Increasing the program break has the effect of 
allocating memory to the process; decreasing the break deallocates 
memory. Return: 0 - OK, -1 - error
NOTE: Not POSIX, but old good legacy Unix
NOTE: used by malloc() & free()
*/
int brk (void *addr)
{
	/*
	Return: new program break on success. On failure (invalid 
	address or no more memory) returns the current break
	*/
	_heap_end = (void *)syscall1 (_SYS_BRK, addr);

	if (_heap_end != addr) {
		errno = ENOMEM;
		return -1;
	}

	return 0;
}

/*
Increments the program break by @inc bytes. On success returns the prev 
program break (you can use @inc=0 to find out the current break). On 
error return -1
NOTE: Not POSIX, but old good legacy Unix
NOTE: used by malloc() and free()
*/
void *sbrk (int inc)
{
	char *p = _heap_end;

	if (brk ((char *)_heap_end + inc)) {
		errno = ENOMEM;
		return (void *)-1;
	}
	return p;
}

/*
Send signal to a process(-es)
* @pid > 0 - send signal to process with PID=@pid
* @pid = 0 - send signal to all processes in group of the calling process
* @pid =-1 - send signal to every process fro which the calling process 
has permission to send signals, except the process 1 (init)
* @pid < 0 - send signal to all processes in group with PGID=-@pid
*/
int kill (int pid, int sig)
{
	return syscall2 (_SYS_KILL, pid, sig);
}

// Change working directory
int chdir (const char *path)
{
	return syscall1 (_SYS_CHDIR, path);
}

/*
Rename file, moving it between directories if required. If @newpath 
already exists, it will be automatically replaced
NOTE: stdio.h
*/
int rename (const char *oldpath, const char *newpath)
{
	return syscall2 (_SYS_RENAME, oldpath, newpath);
}

/*
Return seconds since Epoch (Jan, 1 1970 00:00:00 GMT)
If @t not NULL than store return value also there
NOTE: time.h
*/
int time (int *t)
{
	return syscall1 (_SYS_TIME, t);
}

/*
Creates a copy of @oldfd descriptor, using the lowest-numbered unused 
desc fro the new desc
Return: -1 on error, new desc on OK
*/
int dup (int oldfd)
{
	return syscall1 (_SYS_DUP, oldfd);
}

/*
Do the same thing as dup(), but instead of using the lowest-numbered 
unused desc, it uses the desc num specified in @newfd. If the @newfd was 
previously open, it is silently closed before being reused
Return: -1 on error, new desc on OK
*/
int dup2 (int oldfd, int newfd)
{
	return syscall2 (_SYS_DUP2, oldfd, newfd);
}

/*
Create pipe - unidirectional data channel that can be used for 
interprocess communication. The array @pipefd[] is used to return 2 file 
descriptors referring to the end of the pipe. @pipefd[0] refers to the 
read end of the pipe, @pipefd[1] refers to the write end of the pipe
Return: 0 - OK, -1 - error
*/
int pipe (int pipefd[2])
{
	return syscall1(_SYS_PIPE, pipefd);
}

/*
Truncate file to the specified length.
Return: 0 - OK, -1 - error
*/
int truncate (const char *path, int to_len)
{
	return syscall2 (_SYS_TRUNCATE, path, to_len);
}

int ftruncate (int fd, int to_len)
{
	return syscall2 (_SYS_FTRUNCATE, fd, to_len);
}

/*
Reboot, halt or poweroff system (depends on command). Linux specific 
(not POSIX!). Only root can do that. Normally doesn't return. On error 
return -1 and set errno (EPREM, EINVAL). You must perform sync() before
rebooting otherwise your data may be lost
*/
int reboot (int cmd)
{
	return syscall4 (_SYS_REBOOT, 0xFEE1DEAD, 0x28121969, cmd, 0);
}

/*
Flush inodes and data (buffers) to disks. POSIX says that sync() can 
return before actual flush completes, but Linux waits until work is done 
Usually used before rebooting or poweroff
*/
void sync (void)
{
	syscall0 (_SYS_SYNC);
}

/*
Terminates process. @status - low 8 bits are returned to parent process
*/
void _exit (int status)
{
	syscall1 (_SYS_EXIT, status);
}

/*
Wait child termination and return it's PID and exit status in @status
Return -1 in case of error
*/
int wait (int *status)
{
	return syscall3 (_SYS_WAITPID, -1, status, 0);
}

/*
Wait termination of a child with PID=@pid
If @pid > 0 - it is a PID of child to wait
If @pid == 0 - wait for any child with PGID is equal to the PGID of the 
calling process 
If @pid == -1 - wait for any child process
If @pid < -1 - wait for any child with PGID=@pid
*/
int waitpid (int pid, int *status, int options)
{
	return syscall3 (_SYS_WAITPID, pid, status, options);
}

// Return process PID
int getpid (void)
{
	return syscall0 (_SYS_GETPID);
}

/* Read directory entries */
int getdents (int fd, void *buf, int sz)
{
	return syscall3 (_SYS_GETDENTS, fd, buf, sz);
}

/*
Control device
*/
int ioctl (int fd, int cmd, void *arg)
{
	return syscall3 (_SYS_IOCTL, fd, cmd, arg);
}

/* Get file status info */
int stat (const char *fname, struct stat *p)
{
	return syscall2 (_SYS_STAT, fname, p);
}

/* Remove link to file */
int unlink (const char *fname)
{
	return syscall1 (_SYS_UNLINK, fname);
}

/* Create link to file */
int link (const char *old, const char *new)
{
	return syscall1 (_SYS_LINK, old, new);
}

/* Set atime and mtime of file. If @ptimes == NULL then set them to 
current time */
int utime (const char *fname, const struct utimbuf *ptimes)
{
	return syscall2 (_SYS_UTIME, fname, ptimes);
}

/* Create directory */
int mkdir (const char *path, int mode)
{
	return syscall2 (_SYS_MKDIR, path, mode);
}

/* Set system type (not POSIX?). Only root can do this */
int stime (const time_t *p)
{
	return syscall1 (_SYS_STIME, p);
}

/*
Set owner and group of file. Follow symlinks. Only root can do this
*/
int chown (const char *fname, int uid, int gid)
{
	return syscall3 (_SYS_CHOWN, fname, uid, gid);
}

/*
Set owner and group of file. Don't follow symlinks. Only root can do 
this
@uid and @gid can be -1: in this case this field doesn't change
*/
int lchown (const char *fname, int uid, int gid)
{
	return syscall3 (_SYS_LCHOWN, fname, uid, gid);
}

/*
Set owner and group of the opened file. Only root can do this
*/
int fchown (int fd, int uid, int gid)
{
	return syscall3 (_SYS_FCHOWN, fd, uid, gid);
}

/*
Set file access mode
*/
int chmod (const char *fname, int mode)
{
	return syscall2 (_SYS_CHMOD, fname, mode);
}

int fchmod (int fd, int mode)
{
	return syscall2 (_SYS_FCHMOD, fd, mode);
}

/* Get current working directory of the process. On error return NULL */
char *getcwd (char *buf, int bufsz)
{
	if (syscall2 (_SYS_GETCWD, buf, bufsz) >= 0)
		return buf;
	return (char *)0;
}

/* Get UID, always successful */
int getuid (void)
{
	return syscall0 (_SYS_GETUID);
}

/* Get GID, always successful */
int getgid (void)
{
	return syscall0 (_SYS_GETGID);
}

/*
Ask system to send SIGALRM signal to a caller process 
*/
int alarm (unsigned sec)
{
	return syscall1 (_SYS_ALARM, sec);
}

/*
Set signal handler or to SIG_IGN or SIG_DFL. Return pointer to the 
previous signal handler
*/
int signal (int sig, void (*f) (int))
{
	return syscall2 (_SYS_SIGNAL, sig, f);
}


/* Fill in system info in @p (uname - Unix Name) */
int uname (struct utsname *p)
{
	return syscall1 (_SYS_UNAME, p);
}

/* Set host name (WARNING: not POSIX) */
int sethostname (const char *buf, int sz)
{
	return syscall2 (_SYS_SETHOSTNAME, buf, sz);
}

/* Set domain name (WARNING: not POSIX) */
int setdomainname (const char *buf, int sz)
{
	return syscall2 (_SYS_SETDOMAINNAME, buf, sz);
}

/* Set process signal mask */
int sigprocmask (int how, unsigned *set, unsigned *old)
{
	return syscall3 (_SYS_SIGPROCMASK, how, set, old);
}

/*
Start new session (and new process group in this new session) and 
make the caller process the process leader and the session leader
The new session has not CTTY yet. The first open() on any free TTY
makes this TTY CTTY of this session
*/
int setsid (void)
{
	return syscall0 (_SYS_SETSID);
}

/* Only root can do this */
int setuid (int uid)
{
	return syscall1 (_SYS_SETUID, uid);
}

/* Only root can do this */
int setgid (int gid)
{
	return syscall1 (_SYS_SETGID, gid);
}

/* Wait any signal */
int pause (void)
{
	return syscall0 (_SYS_PAUSE);
}

int mknod (const char *fname, int mode, int dev)
{
	return syscall3 (_SYS_MKNOD, fname, mode, dev);
}


int mount (const char *dev, const char *dir, const char *fs_type, 
unsigned flags, const void *data)
{
	return syscall5 (_SYS_MOUNT, dev, dir, fs_type, flags, data);
}

int umount (const char *dir)
{
	return syscall1 (_SYS_UMOUNT, dir);
}

/*
Set process umask: new files will be created withmode = MODE & ~umask, 
where MODE - mode specified in open syscall. Always succeed. Return 
previous mask
*/
int umask (int mask)
{
	return syscall1 (_SYS_UMASK, mask & 0777);
}
