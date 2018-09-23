#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

extern void *_heap_beg;
extern void *_heap_end;

char **__argv;
char **environ;

void _init_stream (void);
void _fin_stream (void);

void _init (int argc, char *argv[], char *envp[])
{
	__argv = argv;
	environ = envp;
	brk(0); // 0 - invalid addr for brk(), we just find out the
		// current program break
	_heap_beg = _heap_end;
	_init_stream();
}

void exit (int status)
{
	_fin_stream();
	_exit(status); // Exit syscall
}

void perror (const char *s)
{
	fprintf (stderr, "%s: %s\n", s, strerror(errno));
}
