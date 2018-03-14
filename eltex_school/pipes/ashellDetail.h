#ifndef ASHELL_DETAIL_H
#define ASHELL_DETAIL_H

#include <assert.h> /* assert */

#define _GNU_SOURCE
#include <fcntl.h> /* O_* constants */

#include <unistd.h> /* pipe */
#include <stdio.h> /* perror */
#include <stdlib.h> /* calloc */
#include <string.h> /* strpbrk */

#include <sys/types.h> /* wait */
#include <sys/wait.h>  /* wait */


/* 	Get user's command from standard input 
	Return values:
		In success case, 0 is returned;
		Otherwise 1 is returned. */
int getCommand(char* *cmd);


/*	Handles and disassembles complex command and 
	execute its.
	Return values:
		-1 - unsuccessfull command completion,
		 0 - exit command,
		 1 - otherwise */
int handleCommand(char *command);


/*	Saves file descriptors of standard input-output
	in the oldfd[2];
	Is success case, zero is returned. */
int saveIOfd(int old_fd[2]);


/*	Loads standard input-output file descriptors 
	saved previously;
	Is success case, zero is returned. */
int loadIOfd(int old_fd[2]);


/**/
int setIOfd(int first_pipe[2], int second_pipe[2]);


/**/
int executeToken(char *token, int first_pipe[2], int second_pipe[2]);


/**/
int createPipes(int first_pipe[2], int second_pipe[2]);


/**/
void swapPipes(int pipe1[2], int pipe2[2]);

#endif //ASHELL_DETAIL_H