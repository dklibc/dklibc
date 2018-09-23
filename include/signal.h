/*
Signal codes for kill(), signal(), sigaction()
ANSI C
*/

#ifndef _SIGNAL_H
#define _SIGNAL_H

#define SIGHUP	1
#define SIGINT	2
#define SIGQUIT	3
#define SIGILL  4  /* Illegal instruction (ANSI) */
#define SIGABRT 6  /* Abort (ANSI) */
#define SIGFPE  8  /* Floating point exception (ANSI) */
#define SIGKILL	9
#define SIGUSR1 10 /* User defined signal (POSIX) */
#define SIGSEGV 11 /* Segmentation violation (ANSI) */
#define SIGUSR2 12 /* User defined signal #2 (POSIX) */
#define SIGPIPE	13 /* Broken pipe (POSIX) */
#define SIGALRM	14 /* Alarm (POSIX) */
#define SIGTERM	15 /* Ask for termination (POSIX) */
#define SIGCHLD	17 /* Child is terminated (POSIX) */
#define SIGCONT	18
#define SIGSTOP	19
#define SIGTSTP 20
#define SIGIO	29
#define SIGSYS  31 /* Bad sys call */

typedef void (*__sighandler_t) (int);

#define SIG_DFL ((__sighandler_t)0) /* default signal handling */
#define SIG_IGN ((__sighandler_t)1) /* ignore signal */
#define SIG_ERR ((__sighandler_t)-1)

/* Set signal handler */
__sighandler_t signal (int sig, __sighandler_t f);

#define SIG_BLOCK	0
#define SIG_UNBLOCK	1
#define SIG_SETMASK	2

#define sigemptyset(set_ptr) (*set_ptr = 0)
#define sigfillset(set_ptr) (*set_ptr = ~0)
#define sigaddset(set_ptr, sig) (*set_ptr |= (1 << (sig-1)))
#define sigdelset(set_ptr, sig) (*set_ptr &= ~(1 << (sig-1)))

/* Set signal mask */
int sigprocmask (int how, unsigned *set, unsigned *old);

#endif

