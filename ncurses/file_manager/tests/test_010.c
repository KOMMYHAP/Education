#include <assert.h>
#include <stdio.h>
#include "../content.h"

char *generate_string()
{
	char *buffer = (char*)malloc(10);
	if (buffer != NULL) {
		memset(buffer, 42, 9);
		buffer[9] = '\0';
	}
	return buffer;
}

/*
	Необходимо протестировать функцию content_move
	С различным количеством данных (0..1024)
*/
int test()
{
	int result = 0;
	int amount[] = {
		0, 10, 20, 200, 1000, 1000000
	};
	const int N = sizeof(amount) / sizeof(amount[0]);

	content_t content;
	if (content_init(&content) != 0) {
		perror("content_init");
		return -1;
	}

	char *temp;
	for (int epoch = 0; epoch < N; ++epoch) {
		for (int i = 0; i < amount[epoch]; ++i) {
			temp = generate_string();
			if (temp == NULL) {
				return -1;
			}
			if (content_move(&content, temp) != 0) {
				if (result < -9) {
					dprintf(STDERR_FILENO, "[epoch=%d, i=%d]: too much errors\n", epoch, i);
					//free(temp);
					break;
				}
				dprintf(STDERR_FILENO, "[epoch=%d, i=%d]: cannot move %s\n", epoch, i, temp);
				--result;
				//free(temp);
			}
		}
		if (result < 0) {
			dprintf(STDERR_FILENO, "[epoch=%d]: content is not correct(%d or more errors)\n", epoch, -result);
		}
		content_free(&content);
	}
	return result;
}

int main()
{
	assert(test() == 0);
	return 0;
}