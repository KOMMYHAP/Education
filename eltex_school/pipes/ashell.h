#ifndef ASHELL
#define ASHELL

#include "ashellDetail.h"


/* 	Signals handler for SIGCHLD */
void sighandler(int sig);


/*	Initialize AShell by running
	some init functions and
	handling some signal, like 
	SIGCHLD */
void ashellInit();


/*	Runner of interpretator */
void ashellRun();


/* 	Main loop of interpretator.
	Return result of ashellStep() */
int ashellLoop();


/* 	Every iteration of main-loop
	will invoke this function.
	Return values:
		-1 - unsuccessfull step completion,
		 0 - last step completed, end of loop,
		 1 - succeffull step comletion */
int ashellStep();


/* 	@brief: 
	This function will invoked before ashellLoop() 
	starting.

	Prints intro message, like 
	"[author]: <username>,
	 [description]: <some text>,
	 etc..." 
*/
void ashellIntro();


/* 	@brief: 
	This function will invoked is unsuccess case
	completion of ashellLoop().
	@todo: 
	Error backtrace printing. */
void ashellError();


/* 	@brief:
	This function will invoked in success case completion of
	ashellLoop() by using 'exit' command. 
	
	Prints exit message, like "Good luck!".
*/
void ashellExit();

#endif // ASHELL