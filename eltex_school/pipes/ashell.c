#include "ashell.h"
#include "ashellDetail.h"

void sighandler(int sig)
{
	printf("[sighandler]: catched %d\n", sig);
	wait(0);
}

void ashellPrintIntroMessage()
{
	char const author[] = "Polyakov Pavel";
	char const nick[] = "kommyhap";
	char const email[] = "pasha.u.polyak@gmail.com";

	printf("[author]: %s,\n[nick]: %s,\n[email]: %s\n", author, nick, email);
}

void ashellInit()
{
	struct sigaction act;
	act.sa_handler = sighandler;
	sigaction(SIGCHLD, &act, NULL);
}

int ashellStep()
{
	printf("> ");

	char *cmd = NULL;
	int retvalue;

	retvalue = getCommand(&cmd);
	if (retvalue < 0) {
		perror("[ashellStep]");
		return retvalue;
	}

	retvalue = handleCommand(cmd);

	free(cmd);

	return retvalue;
}

void ashellLoop()
{
	ashellPrintIntroMessage();
	while (ashellStep != 0);
}
