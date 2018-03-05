#include <unistd.h> /* pipe */
#include <stdio.h> /*stdin, stdout, fprintf, etc */
#include <stdlib.h> /* exit */

// In success, zero is returned;
int testReading(int *number)
{
	const int retvalue = scanf("%d", number);
	return (retvalue == 1) ? 0 : 1;
}

// In success, zero is returned;
int testWriting(int number)
{
	const int retvalue = printf("%d", number);
	return (retvalue > 0) ? 0 : 1;
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

	if (dup2(2, fd[1]) != fd[1]) {
		perror("[main::dup2 /* write fd*/]");
		exit(1);
	}

	if (dup2(1, fd[0]) != fd[0]) {
		perror("[main::dup2 /* read fd */]");
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
	}
	fprintf(stderr, "\n");
	return 0;
}