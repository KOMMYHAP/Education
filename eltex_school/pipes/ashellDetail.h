#ifndef ASHELL_DETAIL_H
#define ASHELL_DETAIL_H


/*	Handles and disassembles complex command and 
	execute its. */
int handleCommand(char* cmd);


/* 	Get user's command from standard input 
	Example: "ps -ef | grep 2017" */
int getCommand(char* *cmd);


/* 	Parses complex command and put
	first simple command to token.
	Example: 
		after: 
			cmd = "ps -ef | grep 2017",
			token = NULL
		before: 
			cmd = "| grep 2017", 
			token = "ps -ef" 
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