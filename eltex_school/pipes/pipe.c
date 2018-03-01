#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	if (argc < 2) {
		printf("usage: %s <command> [argument]\n", argv[0]);
		exit(EXIT_SUCCESS);
	}

	if (execvp(argv[1], &argv[1]) == -1) {
		perror("[execvp]");
		exit(EXIT_FAILURE);
	}
}