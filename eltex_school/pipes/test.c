#include <unistd.h> /* pipe */
#include <stdio.h> /*stdin, stdout, fprintf, etc */
#include <stdlib.h> /* exit */
#include <string.h> /* strlen */

// In success, zero is returned;
int testReading(int *number)
{
	const int retvalue = read(1, (void*)number, sizeof(*number));
	return (retvalue == sizeof(*number)) ? 0 : 1;
}

// In success, zero is returned;
int testWriting(int number)
{
	const int retvalue = write(2, (void*)&number, sizeof(number));
	return (retvalue == sizeof(number)) ? 0 : 1;
}

// In success, zero is returned;
int testTrivial(int newfd[2])
{
	char msg = 'x';
	char buffer = '\0';

	int const oldfd[2] = {dup(STDIN_FILENO), dup(STDOUT_FILENO)};

	if (dup2(newfd[0], STDIN_FILENO) == -1 || dup2(newfd[1], STDOUT_FILENO) == -1) {
		perror("[testTrivial::dup2]");
		return 1;
	}

	if (write(STDOUT_FILENO, &msg, 1) != 1) {
		perror("[testTrivial::write]");
		return 1;
	}
	if (read(STDIN_FILENO, &buffer, 1) != 1) {
		perror("[testTrivial::read]");
		return 1;
	}

	if (dup2(STDIN_FILENO, oldfd[0]) == -1 || dup2(STDOUT_FILENO, oldfd[1]) == -1) {
		perror("[testTrivial::dup2]");
		return 1;
	}

	dprintf(STDOUT_FILENO, "testTrivial is comleted successfully.\n");

	return (msg == buffer) ? 0 : 1;
}

/* 	In success, zero is returned,
	Writing failed, -1 is returned,
	Reading failed, -2 is returned,
	Equaling failed, -3 is returned;
	*/
int test() 
{
	int const input[] = {0, 1, 0xffff, 42, -42, 0xffffffff, 0x7fffffff};
	int const count = sizeof(input) / sizeof(input[0]);
	int output[count];
	for (int i = 0; i < count; ++i) {
		if (testWriting(input[i]) != 0) {
			return -1;
		}
		if (testReading(&output[i]) != 0) {
			return -2;
		}
		if (input[i] != output[i]) {
			return -3;
		}
	}
	return 0;
}

int main()
{
	int fd[2];
	if (pipe(fd) != 0) {
		perror("[main::pipe]");
		exit(1);
	}

	if (testTrivial(fd) != 0) {
		dprintf(3, "[main::testTrivial]");
		exit(1);
	}

	if (dup2(2, fd[1]) == -1) {
		perror("[main::dup2 /* write fd*/ ]");
		exit(1);
	}

	if (dup2(1, fd[0]) == -1) {
		perror("[main::dup2 /* read fd */ ]");
		exit(1);
	}

	switch(test())
	{
		case 0:
			fprintf(stderr, "[main::test]: read and recorded values are equal.");
			break;
		case -1:
			fprintf(stderr, "[main::test]: writing failed.");
			break;
		case -2:
			fprintf(stderr, "[main::test]: reading failed.");
			break;
		case -3:
			fprintf(stderr, "[main::test]: equaling failed.");
			break;
		default:
			fprintf(stderr, "[main::test]: unknown error.");
	}

	if (close(fd[0]) != 0 || close(fd[1]) != 0) {
		perror("[main::close]");
		exit(1);
	}

	fprintf(stderr, "\n");
	return 0;
}