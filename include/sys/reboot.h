/* Linux-specific: shutdown (reboot, halt or poweroff) a system */

#ifndef _SYS_REBOOT
#define _SYS_REBOOT

/* Commands for reboot() */
#define DO_REBOOT	0x01234567
#define DO_POWEROFF	0x4321fedc
#define DO_HALT		0xcdef0123

int reboot (int cmd);

#endif

