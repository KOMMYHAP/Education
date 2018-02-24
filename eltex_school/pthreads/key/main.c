#include <pthread.h>	/* pthread* */
#include <stdio.h>		/* NULL */
#include <stdlib.h>		/* free */
#include <errno.h> 		/* errno */
#include <string.h>		/* memcpy */
#include <unistd.h>		/* sleep */

#define thread_num 5

static pthread_key_t key;
static pthread_once_t key_once = PTHREAD_ONCE_INIT;

static void make_key();
static void clean(void *);

static void clean(void *data)
{
	printf("[clean]: invoked\n");
	free(data);
}

static void make_key()
{
	printf("[make_key]: invoked\n");
	if (pthread_key_create(&key, clean) != 0) {
		perror("[make_key]");
		pthread_exit((void*)&errno);
	}
}

void* test(void *data)
{
	void *ptr = NULL;
	pthread_once(&key_once, make_key);

	ptr = pthread_getspecific(key);
	if (errno != 0) {
		perror("[test] pthread_getspecific");
		pthread_exit((void*)&errno);
	}

	if (ptr == NULL) {
		ptr = malloc(sizeof(int));

		if (ptr == NULL) {
			perror("[test] malloc");
			pthread_exit(NULL);
		}

		if (pthread_setspecific(key, ptr) != 0) {
			perror("[test] pthread_setspecific");
			pthread_exit((void*)&errno);
		}
	}

	memcpy(ptr, data, sizeof(int));
	//sleep(*(int*)(ptr)); // usefull for check
	printf("%p: %d\n", ptr, *(int*)(ptr));
	return NULL;
}

int main(int argc, char **argv)
{
	pthread_t thread_data[thread_num];
	pthread_t thread_id[thread_num];

	for (int i = 0; i < thread_num; ++i) {
		thread_data[i] = i;
		if (pthread_create(&thread_id[i], NULL, test, (void*)&thread_data[i]) != 0) {
			perror("[main] pthread_create");
		}
	}

	for (int i = 0; i < thread_num; ++i) {
		if (pthread_join(thread_id[i], NULL) != 0) {
			perror("[main] pthread_join");
		}
	}

	return 0;
}