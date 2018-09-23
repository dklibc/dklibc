static char *s[35] = {
/* 0 */	"0",
/* 1 */	"Operation not permitted (EPERM)",
/* 2 */	"No such file or directory (ENOENT)",
/* 3 */	"No such process (ESRCH)",
/* 4 */	"Interrupted system call (EINTR)",
/* 5 */	"I/O error (EIO)",
/* 6 */	"No such device or address (ENXIO)",
/* 7 */	"Argument list too long (E2BIG)",
/* 8 */	"Exec format error (ENOEXEC)",
/* 9 */	"Bad file number (EBADF)",
/* 10*/	"No child processes (ECHLD)",
/* 11*/	"Try again (EAGAIN)",
/* 12*/	"Out of memory (ENOMEM)",
/* 13*/	"Permission denied (EACCES)",
	"Bad address (EFAULT)",
	"Block device required (ENOTBLK)",
	"Device or resource busy (EBUSY)",
	"File exists (EEXIST)",
	"Cross-device link (EXDEV)",
	"No such device (ENODEV)",
	"Not a directory (ENOTDIR)",
	"Is a directory (EISDIR)",
	"Invalid argument (EINVAL)",
	"File table overflow (ENFILE)",
	"Too many open files (EMFILE)",
	"Not a typewriter (ENOTTY)",
	"Text file busy (ETXTBSY)",
	"File too large (EFBIG)",
	"No space left on device (ENOSPC)",
	"Illegal seek (ESPIPE)",
	"Read-only file system EROFS()",
	"Too many links (EMLINK)",
	"Broken pipe (EPIPE)",
	"Math argument out of domain of func (EDOM)",
	"Math result not representable (ERANGE)",
};

char *strerror (int e)
{
	if (e > 34)
		return "Unknown";
	return s[e];
}
