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
	will invoked this function */
void ashellStep();

#endif // ASHELL