/* Macroses for examine child exit status , returned by wait()/waitpid() */

#ifndef _SYS_WAIT_H
#define _SYS_WAIT_H

/* waitpid option flags */
#define WNOHANG 1

/* wait status */

// Process exit code if WIFEXITED(STATUS)
#define	WEXITSTATUS(status) (((status) & 0xff00) >> 8)

// If WIFSIGNALED(STATUS), the terminating signal
#define	WTERMSIG(status) ((status) & 0x7f)

// If WIFSTOPPED(STATUS), the signal that stopped the child
#define	WSTOPSIG(status) WEXITSTATUS(status)

// Nonzero if STATUS indicates normal termination
#define	WIFEXITED(status) (WTERMSIG(status) == 0)

// Nonzero if STATUS indicates termination by a signal
#define WIFSIGNALED(status) (((signed char) (((status) & 0x7f) + 1) >> 1) > 0)

// Nonzero if STATUS indicates the child is stopped
#define	WIFSTOPPED(status) (((status) & 0xff) == 0x7f)

#endif

