#include "test.h"

/*
	@brief: Unique thread key.
*/
static pthread_key_t key;
static pthread_once_t key_once = PTHREAD_ONCE_INIT;

void make_key()
{
	printf("[make_key]: invoked\n");
	if (pthread_key_create(&key, clean) != 0) {
		perror("[make_key]");
		pthread_exit((void*)&errno);
	}
}

void clean(void *data)
{
	printf("[clean]: invoked\n");
	free(data);
}

void* func(void *data)
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

int test()
{
	pthread_t thread_data[thread_num];
	pthread_t thread_id[thread_num];
	int ferror = 0;

	for (int i = 0; i < thread_num; ++i) {
		thread_data[i] = i;
		if (pthread_create(&thread_id[i], NULL, func, (void*)&thread_data[i]) != 0) {
			perror("[main] pthread_create");
			ferror = 1;
		}
	}

	for (int i = 0; i < thread_num; ++i) {
		if (pthread_join(thread_id[i], NULL) != 0) {
			perror("[main] pthread_join");
			ferror = 1;
		}
	}

	return ferror;
}
