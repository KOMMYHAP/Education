#include <assert.h>
#include "../content_detail.h"

void test()
{
	char const *true_p[] = {
		"/a/b/c/d/e/f/g/h",
		"/home/kommyhap",
		"/home",
	};
	char const *false_p[] = {
		"/",
		"..",
		"/home/",
		"word",
		NULL
	};

	char *temp;
	for (int i = 0; i < sizeof(true_p) / sizeof(true_p[0]); ++i) {
		temp = get_previous_dir(true_p[i]);
		assert(temp != NULL);
		free(temp);
	}

	for (int i = 0; i < sizeof(false_p) / sizeof(false_p[0]); ++i) {
		temp = get_previous_dir(false_p[i]);
		assert(temp == NULL);
		free(temp);
	}

}

int main()
{
	test();
	return 0;
}