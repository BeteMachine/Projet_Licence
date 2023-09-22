#include "exact_rw.h"

ssize_t exact_write(int fd, void *buf, size_t nbytes)
{
	ssize_t n;
	size_t total = 0;
	while (total < nbytes)
	{
		n = write(fd, (char *) buf + total, nbytes - total);
		if (n == -1) /* error */
		return n;
		total += n;
	}
	return total;
}

ssize_t exact_read(int fd, void *buf, size_t nbytes)
{
	ssize_t n;
	size_t total = 0;
	while (total < nbytes)
	{
		n = read(fd, (char *) buf + total, nbytes - total);
		if (n == -1) /* error */
		return n;
		if (n == 0) /* end of file */
		break;
		total += n;
	}
	return total;
}
