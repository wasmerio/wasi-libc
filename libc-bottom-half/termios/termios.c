// Userspace emulation of termios

#define _WASI_EMULATED_TERMIOS
#include <termios.h>
#include <errno.h>

int tcsetattr(int fd, int act, const struct termios *tio)
{
	errno = EINVAL;
	return -1;
}

int tcgetattr(int fd, struct termios *tio)
{
	errno = EINVAL;
	return -1;
}
