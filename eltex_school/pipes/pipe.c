#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

void sighandler(int sig)
{
	printf("[sighandler]: catched %d\n", sig);
	wait(0);
}

int execute(char const *file, char *argv[])
{
	pid_t pid;
	pid = fork();
	if (pid == -1) {
		perror("[fork]");
		return -1;
	}
	if (pid == 0) { // потомок
		if (execvp(file, argv) == -1) {
			perror("[execvp]");
			exit(EXIT_FAILURE);
		}
	}
	return pid;
}

int main(int argc, char *argv[])
{
	if (argc < 2) {
		printf("Usage: %s <command> [argument]\n", argv[0]);
		exit(EXIT_SUCCESS);
	}

	struct sigaction act;
	act.sa_handler = sighandler;
	sigaction(SIGCHLD, &act, NULL);

	pid_t pid = execute(argv[1], &argv[1]);
	
	if (pid > 0) {
		printf("Child created with %d pid.\n", pid);
	}
	wait(0);
	return 0;
}