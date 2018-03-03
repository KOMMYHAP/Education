#ifndef ASHELL
#define ASHELL

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

/* 	Signals handler for SIGCHLD */
void sighandler(int sig);

/* 	Print intro message, like 
	"[author]: <username>,
	 [description]: <some text>,
	 etc..." */
void ashellPrintIntroMessage();

/*	Initialize AShell by running
	some init functions and
	handling some signal, like 
	SIGCHLD */
void ashellInit();

/* 	Main loop for shell */
void ashellLoop();

/* 	Every iteration of main-loop
	will invoked this function.
	Return values:
		-1 - unsuccessfull step completion,
		 0 - last step completed, end of loop,
		 1 - succeffull step comletion */
int ashellStep();

#endif // ASHELL