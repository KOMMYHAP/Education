#include "content_detail.h"

int readln(int fd, char* *message_ptr) 
{
	if (message_ptr == NULL || fd < 0) {
		errno = EINVAL;
		return -1;
	}

	char *msg = NULL, *temp = NULL, c = 0;
	size_t size = 1;

	int retvalue = 0;
	do {
		retvalue = read(fd, &c, 1);
		if (retvalue == 1) { // correct symbol is gotten
			if (c == '\n') {
				break;
			}
			
			temp = msg; // save old msg

			msg = (char*)malloc(size + 1); // alloc new space
			if (msg == NULL) {
				dprintf(STDERR_FILENO, "[readln]: cannot allocate %ld bytes\n", size + 1);
				break;
			}

			if (temp != NULL) {
				memcpy(msg, temp, size); // copy old msg to new space
				free(temp); // free old msg
			}

			msg[size - 1] = c;
			msg[size] = '\0';
			++size;
		} else if (retvalue == -1 && errno == EAGAIN) { // EOF is reached (not error)
			retvalue = 0;
		}
	} while (retvalue == 1);

	*message_ptr = msg;
	return retvalue;
}

int is_root(const char *path)
{
	return !!path && path[0] == '/' && path[1] == '\0'; /* checks on non-zero and on "/" path */
}

char *get_previous_dir(const char *path)
{
	if (path != NULL && !is_root(path)) {
		// check path:
		const int path_len = strlen(path);
		if (path[path_len - 1] == '/') {
			errno = EINVAL;
			dprintf(STDERR_FILENO, "[get_previous_dir]: cannot get previous directory from \"%s\"\n", path);
			return NULL;
		}

		// поиск '/' начиная с конца
		char *ptr = strrchr(path, '/');
		if (ptr == NULL) {
			// по какой-то причине path (абсолютный путь) не содержит '/'
			errno = EINVAL;
			dprintf(STDERR_FILENO, "[get_previous_dir]: cannot get previous directory from \"%s\"\n", path);
			return NULL;
		}

		int const buffer_len = (int)(ptr - path); // "/home/kommyhap" -> buffer_len = 5
		char *buffer = (char*)malloc(buffer_len + 1); // buffer = "/home\0"
		if (buffer == NULL) {
			errno = ENOMEM;
			dprintf(STDERR_FILENO, "[get_previous_dir]: cannot allocate %d bytes\n", buffer_len + 1);
			return NULL;
		}
		memcpy(buffer, path, buffer_len);
		buffer[buffer_len] = '\0';

		return buffer;
	}
	errno = EINVAL;
	dprintf(STDERR_FILENO, "[get_previous_dir]: cannot get previous directory from \"%s\"\n", path);
	return NULL;
}

char *get_next_dir(const char *path, const char *dir)
{
	if (!!path && !!dir) {
		char *ptr = strrchr(path, '/');
		if (ptr ==  NULL) {
			// по какой-то причине path (абсолютный путь) не содержит '/'
			errno = EINVAL;
			dprintf(STDERR_FILENO, "[get_next_dir]: cannot get next directory with \"%s\"\n", path);
			return NULL;
		}

		const int
			path_len = strlen(path),
			dir_len  = strlen(dir);

		if (dir[dir_len - 1] == '/' || !is_root(path) && path[path_len - 1] == '/') {
			// по какой-то причине next_dir содержит '/' в конце
			// либо path содержит в конце '/', но при этом не является корнем
			errno = EINVAL;
			dprintf(STDERR_FILENO, 
				"[get_next_dir]: " \
				"\"%s\" and  \"%s\" cannot not contain symbol '/' at the end\n", 
				path, dir);
			return NULL;
		}

		const int // length of copy path may be shorter than clearly length of path
			path_copy_len = path_len - !!is_root(path);

		char *buffer = (char*)malloc(path_copy_len + dir_len + 2); // + 1 ('/') + 1 ('\0')
		if (buffer == NULL) {
			errno = ENOMEM;
			dprintf(STDERR_FILENO, 
				"[get_next_dir]: cannot allocate %d bytes\n", 
				path_len + dir_len + 1);
			return NULL;
		}

		memcpy(buffer, path, path_copy_len);
		buffer[path_copy_len] = '/';
		memcpy(buffer + path_copy_len + 1, dir, dir_len);
		buffer[path_copy_len + dir_len + 1] = '\0';

		return buffer;
	}

	errno = EINVAL;
	dprintf(STDERR_FILENO, 
		"[get_next_dir]: " \
		"invalid arguments are current path - \"%s\" " \
		"and next_dir - \"%s\"\n", path, dir);
	return NULL;
}