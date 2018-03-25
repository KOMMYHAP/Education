#include <ncurses.h>
#include <stdlib.h> /* exit */
#include <stdio.h>	/* fgetc */

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
/* qew eq ee weqe eqwew ewq */
	initscr(); // start curses mode
	start_color();
	init_pair(1, COLOR_GREEN, COLOR_BLACK);
	init_pair(2, COLOR_BLACK, COLOR_WHITE);

	int x = 0, y = 0;
	int max_x = 0, max_y = 0;
	getmaxyx(stdscr, max_y, max_x);

	int prev = EOF, ch = fgetc(fp);
	while (ch != EOF) { // read the file till we reach the end
		if (prev == '/' && ch == '*' && x != 0) {
			attron(A_BOLD | COLOR_PAIR(1));
			mvprintw(y, x - 1, "/");
		}

		printw("%c", ch);

		if (prev == '*' && ch == '/') {
			attroff(A_BOLD | COLOR_PAIR(1));
		}

		prev = ch;
		ch = fgetc(fp);
		getyx(stdscr, y, x);

		if (y + 1 == max_y) {
			attron(A_UNDERLINE | COLOR_PAIR(2));

			printw("Press any key to read next page.");
			getch();
			erase();
			refresh();	

			attroff(A_UNDERLINE | COLOR_PAIR(2));
		}
	}

	getch();
	endwin();
	fclose(fp);

	return 0;
}