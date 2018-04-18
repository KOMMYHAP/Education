#include <assert.h>
#include "../content_detail.h"

#define SIZE(X) sizeof(X) / sizeof(X[0])

int test()
{
	const char *path[] = {
		"/",
		"/home/kommyhap"
	};
	const int path_n = SIZE(path);

	const char *next[] = {
		"temp",
		"a/b/c/d/e/f"
	};
	const int next_n = SIZE(next);

	char *temp;
	int result = path_n * next_n;
	for (int i = 0; i < path_n; ++i) {
		for (int j = 0; j < next_n; ++j) {
			temp = get_next_dir(path[i], next[j]);
			if (temp == NULL) {
				dprintf(STDERR_FILENO, 
					"[get_next_dir]: cannot get next dirictory with parameters " \
					"\"%s\" and \"%s\"\n",
					path[i], next[j]);
				continue;
			}
			//printf("#%d: %s\n", i * path_n + j + 1, temp);
			free(temp);
			--result;
		}
	}
	return result;
}

int main()
{
	assert(test() == 0);
	return 0;
}