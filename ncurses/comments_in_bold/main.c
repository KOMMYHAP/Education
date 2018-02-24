#include <ncurses.h>
#include <stdlib.h> /* exit */
#include <stdio.h>	/* fgetc */

#define C_COMMENT 1
#define C_DEFAULT 2

void next_page()
{
	printw("<----Press any key---->");
	getch();
	clear();
	move(0, 0);
}

void start_comment(int x, int y)
{
	attron(A_BOLD | COLOR_PAIR(C_COMMENT));
	move(y, x - 1);
	printw("/*");
}

void end_comment(int x, int y)
{
	move(y, x - 1);
	printw("*/");	
	attroff(A_BOLD | COLOR_PAIR(C_COMMENT));
}

int main(int argc, char **argv)
{
	if (argc != 2) {
		printf("Usage: %s <a c file name>\n", argv[0]);
		exit(1);
	}

	FILE *fp = fopen(argv[1], "r");

	if (fp == NULL) {
		printf("Cannot open file %s/%s\n", argv[0], argv[1]);
		exit(1);
	}

	initscr(); // start curses mode
	start_color();
	init_pair(C_COMMENT, COLOR_GREEN, COLOR_BLACK);
	init_pair(C_DEFAULT, COLOR_WHITE, COLOR_BLACK);

	int y, x, row, col;
	getyx(stdscr, y, x); 
	getmaxyx(stdscr, row, col);

	int quote_counter = 0;
	int prev = EOF, ch = fgetc(fp);
	while (ch != EOF) { // read the file till we reach the end
		getyx(stdscr, y, x); // get current cursor position
		if (ch == '"') {
			quote_counter += 1;
			quote_counter %= 2;
		}
		if (y == (row - 1)) { // are we are at the end of the screen
			next_page();
		}
		if (prev == '/' && ch == '*' && quote_counter == 0) {
			start_comment(x, y);
		} else if (prev == '*' && ch == '/' && quote_counter == 0) {
			end_comment(x, y);
		} else { // not comment borders
			printw("%c", ch);
		}

		prev = ch;
		ch = fgetc(fp); // get next symbol
	}

	getch();
	endwin();
	fclose(fp);

	return 0;
}