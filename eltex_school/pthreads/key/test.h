#ifndef TEST_H
#define TEST_H

#include <pthread.h>	/* pthread */
#include <stdio.h>		/* NULL */
#include <stdlib.h>		/* free */
#include <errno.h> 		/* errno */
#include <string.h>		/* memcpy */
#include <unistd.h>		/* sleep */

#define thread_num 5

/*
	@brief: Creates the key for further allocating thread specific data
*/
void make_key();

/*
	@brief: Deallocates memory of tsd and prints this
*/
void clean(void *);

/*
	@brief: Threads will invoke this function
*/
void* func(void *);

/*
	@brief: Main function that runs test
	Returns zero in success and one otherwise.
*/
int test();

#endif // TEST_H