/* For stat() */

#ifndef _STAT_H
#define _STAT_H

/* File mode bits */

#define S_IFMT		0170000		/* file type bitmask */

/* File types */
#define S_IFSOCK	0140000		/* socket */
#define S_IFLNK		0120000		/* link */
#define S_IFREG		0100000		/* regular file */
#define S_IFBLK		0060000		/* block device */
#define S_IFDIR		0040000		/* directory */
#define S_IFCHR		0020000		/* char device */
#define S_IFIFO		0010000		/* FIFO */

/* Access permissions */

/* Owner access permissions */
#define S_IRWXU		0700	/* mask for owner permisions */
#define S_IRUSR		0400	/* owner has read permission */
#define S_IWUSR		0200	/* owner has write permission */
#define S_IXUSR		0100	/* owner has execute/list permission */

/* Group access permissions */
#define S_IRWXG		0070	/* mask for group permissions */
#define S_IRGRP		0040	/* group has read permission */
#define S_IWGRP		0020	/* group has write permission */
#define S_IXGRP		0010	/* group has execute/list permission */

/* Other access permissions */
#define S_IRWXO		0007	/* mask for other permissions */
#define S_IROTH		0004	/* other has read permission */
#define S_IWOTH		0002	/* other has write permission */
#define S_IXOTH		0001	/* other has execute/list permission */

#define S_ISREG(m) (((m) & S_IFMT) == S_IFREG)
#define S_ISDIR(m) (((m) & S_IFMT) == S_IFDIR)
#define S_ISCHR(m) (((m) & S_IFMT) == S_IFCHR)
#define S_ISBLK(m) (((m) & S_IFMT) == S_IFBLK)
#define S_ISFIFO(m) (((m) & S_IFMT) == S_IFIFO)
#define S_ISLNK(m) (((m) & S_IFMT) == S_IFLNK)
#define S_ISSOCK(m) (((m) & S_IFMT) == S_IFSOCK)

struct stat {
	unsigned short st_dev;
	unsigned short __pad1;
	unsigned st_ino;
	unsigned short st_mode;
	unsigned short st_nlink;
	unsigned short st_uid;
	unsigned short st_gid;
	unsigned short st_rdev;
	unsigned short __pad2;
	unsigned st_size;
	unsigned st_blksize;
	unsigned st_blocks;
	unsigned st_atime;
	unsigned __unused1;	// For nanosec part of atime
	unsigned st_mtime;
	unsigned __unused2;	// For nanosec part of mtime
	unsigned st_ctime;
	unsigned __unused3;	// For nanosec part of ctime
	unsigned __unused4;
	unsigned __unused5;
};

#endif
