#include <unistd.h>
#include <assert.h>

#include "../content_detail.h" /* readln */

int test()
{
	char const *input_msg[] = {
		"qwerty\n",
		"\n",
		"1 2 3 ,/. \n",
		"i'm ",
		"alive!\n"
	};

	int fd[2], retvalue = 0;
	if (pipe(fd) != 0) {
		perror("[pipe]");
		return -1;
	}

	for (int i = 0; i < sizeof(input_msg) / sizeof(input_msg[0]); ++i) {
		const int msg_len = strlen(input_msg[i]);
		if (write(fd[1], input_msg[i], msg_len) < msg_len) {
			perror("[write]");
			return -1;
		}
	}

	if (fcntl(fd[0], F_SETFL, O_NONBLOCK) != 0) {
		perror("[fcntl]");
		return -1;
	}

	char *temp = NULL;
	do {
		retvalue = readln(fd[0], &temp);
		if (retvalue == 1) {
			printf("[%s]\n", temp);
		}
		free(temp);

	} while (retvalue == 1);

	return retvalue;
}

int main()
{
	assert(test() == 0);
	return 0;
}