#define _GNU_SOURCE
#include <fcntl.h> /* O_* constants */

#include <unistd.h> /* pipe */
#include <stdio.h> /* perror */
#include <stdlib.h> /* calloc */
#include <string.h> /* strpbrk */

#include <sys/types.h>
#include <sys/wait.h>

int saveIOfd(int old_fd[2])
{
	old_fd[0] = dup(STDIN_FILENO);
	if (old_fd[0] == -1) {
		perror("[saveIOfd::dup]");
		return -1;
	}

	old_fd[1] = dup(STDOUT_FILENO);
	if (old_fd[1] == -1) {
		perror("[saveIOfd::dup]");
		return -1;
	}

	return 0;
}

int loadIOfd(int old_fd[2])
{
	if (dup2(old_fd[0], STDIN_FILENO) == -1) {
		perror("[loadIOfd::dup2]");
		return -1;
	}

	if (dup2(old_fd[1], STDOUT_FILENO) == -1) {
		perror("[loadIOfd::dup2]");
		return -1;
	}

	return 0;
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

/* Token is c-string like "ls -la" or "find / -name '.so'"
*/
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

void swap(int pipe1[2], int pipe2[2])
{
	int temp;
	for (int i = 0; i < 2; ++i) {
		temp = pipe1[i];
		pipe1[i] = pipe2[i];
		pipe2[i] = temp;
	}
}

int handleCommand(char *command)
{
	int old_fd[2], // standard IO fd
		first_pipe[2], // writing From parent to child;
		second_pipe[2]; // writing from child to parent;

	if (saveIOfd(old_fd) != 0) {
		return 1;
	}

	if (createPipes(first_pipe, second_pipe) != 0) {
		return 1;
	}

	char *token = strtok(command, "|");
	char *next_token;
	int counter = 0;
	while (token != NULL) {
		next_token = strtok(NULL, "|");
		/* For debug only */
		fprintf(stderr, "[%d]: %s\n", ++counter, token);

		/* 	Before the last token execution need change
			outpud fd */
		if (next_token == NULL) {
			second_pipe[1] = old_fd[1];
		}

		if (executeToken(token, first_pipe, second_pipe) != 0) {
			return 1;
		}

		/* 	Меняем местами каналы, чтобы следующая команда читала
			данные, лежащие во втором канале, как если бы они лежали
			в первом.
			Т.о. избегаем копирования данных из одного канала в другой */
		swap(first_pipe, second_pipe);

		// token update
		token = next_token;
	}// end of while

	if (loadIOfd(old_fd) != 0) {
		return 1;
	}

	return 0;
}

int main()
{
	char cmd[] = "ls -la | less";
	printf("<--[");
	int status = handleCommand(cmd);
	printf("%d]-->\n", status);
	return 0;
}