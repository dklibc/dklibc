/*
Linux specific: Mount/unmount FS. These functions are not included in 
POSIX, but are legacy Unix syscalls
*/

#ifndef _SYS_MOUNT_H
#define _SYS_MOUNT_H

/* FS independant mount flags */
#define MS_RDONLY	1
#define MS_NODEV	4
#define MS_NOEXEC	8
#define MS_REMOUNT	32

int mount (const char *dev, const char *dir, const char *type, unsigned flags,
const void *data);
int umount (const char *dir);

#endif

