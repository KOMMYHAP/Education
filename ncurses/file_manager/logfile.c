#include "logfile.h"

int logfile_create(const char *filename)
{
	return creat(filename, 0644);
}

int logfile_init(const char *filename)
{
	int	fd = logfile_create(filename);
	if (fd == -1) {
		perror("[logfile: creat]");
		return -1;
	}
	if (dup2(fd, STDERR_FILENO) == -1) {
		perror("[logfile: dup2]");
		return -1;
	}
	return fd;
}