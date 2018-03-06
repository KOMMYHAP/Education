#ifndef ASHELL_DETAIL_H
#define ASHELL_DETAIL_H

/* 	Get user's command from standard input 
	Example: "ps -ef | grep 2017" */
int getCommand(char* *cmd);


/*	Handles and disassembles complex command and 
	execute its.
	Return values:
		-1 - unsuccessfull command completion,
		 0 - exit command,
		 1 - otherwise */
int handleCommand(char* cmd);

/*	Saves file descriptors of standard input-output
	in the oldfd[2]	*/
void saveIOfd(int oldfd[2]);


/*	Loads standard input-output file descriptors 
	saved previously */
void loadIOfd(int oldfd[2]);


/* 	Parses complex command and put
	first simple (without pipe) command to token.
	Example: 
		before: 
			cmd = "ps -ef | grep root | grep 8311",
			token = NULL
		after 1 iter: 
			cmd = "grep root | grep 8311", 
			token = "ps -ef"
		after 2 iter:
			cmd = "grep 8311",
			token = "grep root" 
		after 3 iter:
			cmd = NULL,
			token = "grep 8311"
*/
int parseCommand(char* cmd, char* *token);


/* 	Executes simple command, like "ps -ef".
	Return > 0 if successfully completed. */
int execute(char *token);


/*	Parses simple command to token.
	Accessory function for execute().
	Example:
		after:
			token = "ps -ef", 
			argv = NULL
		before:
			token = NULL,
			argv = {"ps", "-ef"} */
void parseToken(char* token, char* **argv);


#endif //ASHELL_DETAIL_H