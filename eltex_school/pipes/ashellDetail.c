#include "ashellDetail.h"

int handleCommand(char* command)
{
	if (strcmp(command, "exit") == 0) {
		return 0;
	}

	int old_fd[2], // standard IO fd
		first_pipe[2], // writing From parent to child;
		second_pipe[2]; // writing from child to parent;

	if (saveIOfd(old_fd) != 0) {
		return -1;
	}
	if (createPipes(first_pipe, second_pipe) != 0) {
		return -1;
	}

	char *token = strtok(command, "|");
	char *next_token;
	while (token != NULL) {
		next_token = strtok(NULL, "|");

		/* 	Before the last token execution need change
			outpud fd */
		if (next_token == NULL) {
			second_pipe[1] = old_fd[1];
		}

		if (executeToken(token, first_pipe, second_pipe) != 0) {
			return -1;
		}

		/* 	Меняем местами каналы, чтобы следующая команда читала
			данные, лежащие во втором канале, как если бы они лежали
			в первом.
			Т.о. избегаем копирования данных из одного канала в другой */
		swapPipes(first_pipe, second_pipe);

		// token update
		token = next_token;
	}// end of while

	if (loadIOfd(old_fd) != 0) {
		return -1;
	}

	return 1;
}

int saveIOfd(int oldfd[2])
{
	oldfd[0] = dup(STDIN_FILENO);
	if (oldfd[0] == -1) {
		perror("[saveIOfd::dup]");
		return -1;
	}

	oldfd[1] = dup(STDOUT_FILENO);
	if (oldfd[1] == -1) {
		perror("[saveIOfd::dup]");
		return -1;
	}

	return 0;
}

int loadIOfd(int oldfd[2])
{
	if (dup2(oldfd[0], STDIN_FILENO) == -1) {
		perror("[loadIOfd::dup2]");
		return -1;
	}

	if (dup2(oldfd[1], STDOUT_FILENO) == -1) {
		perror("[loadIOfd::dup2]");
		return -1;
	}

	return 0;
}

int getCommand(char* *cmd)
{
	if (cmd == NULL) {
		fprintf(stderr, "[getCommand]: incorrect argument\n");
		return 1;
	}

	char *result = NULL, *buffer = NULL;
	size_t size = 0, block_size = 64;

	/* 	must be more than one in order to be able 
		to store new char */
	assert(block_size > 0);

	int bytes_read;
	do {
		buffer = (char*)calloc(size + block_size + 1, sizeof(char));
		if (result != NULL) {
			memcpy(buffer, result, size);
			free(result);
		}

		result = buffer;

		bytes_read = read(STDIN_FILENO, result + size, block_size);
		result[size + bytes_read] = '\0';
		if (bytes_read < 0) {
			perror("[getCommand::read]");
			free(result);
			return 1;
		}

		if (result[size + bytes_read - 1] == '\n') {
			result[size + bytes_read - 1] = '\0';
			break;	
		}

		size += bytes_read;
	} while (bytes_read > 0);

	*cmd = result;
	return 0;
}

int executeToken(char *token, int first_pipe[2], int second_pipe[2])
{
	pid_t pid = fork();
	if (pid == -1) {
		perror("[execute::fork]");
		return 1;
	}

	if (pid == 0) { // child
		if (setIOfd(first_pipe, second_pipe) != 0) {
			_exit(1);
		}
		/* 	К моменту выполнения очередной команды, все
			входные данные уже должны быть помещены
			в STDIN_FILENO */
		if (execl("/bin/sh", "/bin/sh", "-c", token, NULL) != 0) {
			_exit(1);
		}
	}
	int status = 0;
	wait(&status);

	return status;
}

int createPipes(int first_pipe[2], int second_pipe[2])
{
	if (pipe(first_pipe) != 0) {
		perror("[createPipes]");
		return 1;
	}

	if (pipe(second_pipe) != 0) {
		perror("[createPipes::pipe]");
		return 1;
	}

	return 0;
}

void swapPipes(int pipe1[2], int pipe2[2])
{
	int temp;
	for (int i = 0; i < 2; ++i) {
		temp = pipe1[i];
		pipe1[i] = pipe2[i];
		pipe2[i] = temp;
	}
}

int setIOfd(int first_pipe[2], int second_pipe[2])
{
	if (close(first_pipe[1]) == -1) {
		perror("[setIOfd::close]");
		return 1;
	}

	if (dup2(first_pipe[0], STDIN_FILENO) == -1) {
		perror("[setIOfd::dup2]");
		return 1;
	}

	if (dup2(second_pipe[1], STDOUT_FILENO) == -1) {
		perror("[setIOfd::dup2]");
		return 1;
	}

	if (close(second_pipe[0]) == -1) {
		perror("[setIOfd::close]");
		return 1;
	}

	return 0;
}