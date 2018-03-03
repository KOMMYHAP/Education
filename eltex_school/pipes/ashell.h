#ifndef ASHELL
#define ASHELL

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