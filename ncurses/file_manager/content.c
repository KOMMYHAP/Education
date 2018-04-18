#include "content.h"


int content_load(const char *path, content_t *content_ptr)
{
	int old_stdout = dup(STDOUT_FILENO);
	if (old_stdout == -1) {
		perror("[dup]");
		return -1;
	}

	int fd[2];
	if (pipe(fd) != 0) {
		perror("[pipe]");
		return -1;
	}

	pid_t pid;
	pid = fork();
	if (pid == -1) {
		perror("[fork]");
		return -1;
	}

	if (pid == 0) { // child
		if (dup2(fd[1], STDOUT_FILENO) == -1) {
			perror("[dup2]");
			_exit(2);
		}
		if (execlp("ls", "ls", "-1", path, (char*)NULL) != 0) {
			perror("[execlp]");
			_exit(2);
		}
	} else { // parent
		int status = 0;
		if (waitpid(pid, &status, 0) != pid) {
			perror("[waitpid]");
			return -1;
		}

		if (status == 0) {
			// save all results to content
			if (fcntl(fd[0], F_SETFL, O_NONBLOCK) != 0) {
				perror("[fcntl]");
				return -1;
			}

			char *buffer = NULL;
			do {
				status = readln(fd[0], &buffer);
				if (status == 1) {
					if (content_move(content_ptr, buffer) != 0) {
						dprintf(STDERR_FILENO, 
							"[content_move]: cannot move \"%s\" to the content\n", 
							buffer);
						free(buffer);
						break;
					} 
				}
			} while (status == 1);

			if (status == -1) {
				perror("[readln]");
				return -1;
			}

			return 0;
		}
		dprintf(STDERR_FILENO, 
			"[ls]: %d is returned with arguments path=%s\n", 
			status, path);
		return -1;
	}
}

int content_move(content_t *content_ptr, char *msg)
{
	if (content_ptr == NULL || msg == NULL) {
		errno = EINVAL;
		return -1;
	}

	if (content_ptr->capacity <= content_ptr->size) {
		// set alias 
		const int cap = content_ptr->capacity;

		// set current step 
		int step = (cap != 0) ? cap : 16; // step == capacity
		if (cap > INT_MAX - step) { // check on overflow
			step = 16; // if overflow is possible need decrease step
			if (cap > INT_MAX - step) { // check on overflow
				errno = EOVERFLOW;
				return -1;
			}
		}
		content_ptr->capacity += step;

		// try to allocate memmory
		char **strings = 
			(char**)malloc(content_ptr->capacity * sizeof(char*));
		if (strings == NULL) {
			errno = ENOMEM;
			return -1;
		}

		// copy old values
		if (content_ptr->strings != NULL) {
			memcpy(strings, content_ptr->strings, sizeof(char*) * content_ptr->size);
			free(content_ptr->strings);
		}
		content_ptr->strings = strings;
	}

	content_ptr->strings[content_ptr->size] = msg;
	content_ptr->size += 1;

	return 0;
}

int content_init(content_t *content_ptr)
{
	if (content_ptr != NULL) {
		memset(content_ptr, 0, sizeof(content_t));
		return 0;
	}
	errno = EINVAL;
	return -1;
}

void content_free(content_t *content_ptr)
{
	if (content_ptr != NULL) {
		for (int i = 0; i < content_ptr->size; ++i) {
			free(content_ptr->strings[i]);
		}
		free(content_ptr->strings);
		memset(content_ptr, 0, sizeof(content_t));
	}
}

char const *content_get(content_t const *content_ptr, int index) 
{
	if (index < content_ptr->size) {
		return content_ptr->strings[index];
	}
	errno = EINVAL;
	return NULL;
}

int init_contents(content_t **contents_ptrs, int n, char const *path)
{
	if (contents_ptrs != NULL && path != NULL && n == 3) {
		int retvalue;
		char *temp;
		char const *ctemp;
		enum {prev = 0, cur, next};
		content_t * const dir[3] = {
			contents_ptrs[prev], 
			contents_ptrs[cur ], 
			contents_ptrs[next]
		};
		
		// initialize pages[prev]
		if (content_init(dir[prev]) != 0) {
			goto free_label_0;
		}
		temp = get_previous_dir(path);
		if (!!temp) {
			retvalue = content_load(temp, dir[prev]);
			free(temp);
			if (retvalue != 0) {
				goto free_label_1;
			}
		}

		// initialize pages[cur ]
		if (content_init(dir[cur ]) != 0) {
			goto free_label_1;
		}
		if (content_load(path, dir[cur ]) != 0) {
			goto free_label_2;
		}

		// initialize pages[next]
		if (content_init(dir[next]) != 0) {
			goto free_label_2;
		}
		ctemp = content_get(dir[cur ], dir[cur]->offset);
		temp = get_next_dir(path, ctemp);
		if (!!temp) {
			retvalue = content_load(temp, dir[next]);
			free(temp);
			if (retvalue != 0) {
				goto free_label_3;
			}
		}

		return 0;

	free_label_3:
		content_free(dir[next]);
	free_label_2:
		content_free(dir[cur ]);
	free_label_1:
		content_free(dir[prev]);
	free_label_0:
		return -1;
	}
	errno = EINVAL;
	return -1;
}