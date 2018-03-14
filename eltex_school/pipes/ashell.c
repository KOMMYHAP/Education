#include "ashell.h"

void sighandler(int sig)
{
	printf("[sighandler]: catched %d\n", sig);
	wait(NULL);
}

void ashellError()
{
	//TODO: error backtrace
}

void ashellExit()
{
	printf("Good luck!\n");
}

void ashellIntro()
{
	char const author[] = "Polyakov Pavel";
	char const nick[] = "kommyhap";
	char const email[] = "pasha.u.polyak@gmail.com";

	printf("[author]: %s\n[nick]: %s\n[email]: %s\n\n", author, nick, email);
}

void ashellInit()
{
	/*
	struct sigaction act;
	act.sa_handler = sighandler;
	sigaction(SIGCHLD, &act, NULL);
	*/
}

int ashellStep()
{
	printf("> ");
	fflush(stdout);

	int status = 0;

	char *cmd = NULL;
	if (getCommand(&cmd) != 0) {
		perror("[ashellStep::getCommand]");
		return -1;
	}

	int retvalue = handleCommand(cmd);
	switch(retvalue)
	{
		case -1: // unsuccess
			fprintf(stderr, "[ashellStep]: unsuccessfull command completion\n");
			status = -1;
			break;
		case 0: // exit command
			status = 0;
			break;
		default: // othewise
			status = 1;
			;
	} // end of switch(retvalue)

	free(cmd);	

	return status;
}

int ashellLoop() 
{
	int status;
	while ( (status = ashellStep()) == 1);
	return status;
}

void ashellRun()
{
	ashellInit();

	ashellIntro();

	int status = ashellLoop();

	switch(status) {
		case -1:
			ashellError();
			break;
		case 0:
			ashellExit();
			break;
		default: 
			;
	} // end of switch
}