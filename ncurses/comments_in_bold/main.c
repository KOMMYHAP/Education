#include <ncurses.h>
#include <stdlib.h> /* exit */
#include <stdio.h>	/* fgetc */

#include "comment_manager.h"

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

	int prev = EOF, ch = fgetc(fp);
	mask_t mask = 0;
	while (ch != EOF) { // read the file till we reach the end
		if (need_next_page()) { // are we are at the end of the screen
			set_next_page();
		}

		mask = is_comment_border();

		if (is_comment_border(mask)) {
			set_start_comment();
			printw("%c", ch);
		}
		if (is_end_comment()) {
			set_end_commnet();
		}

		prev = ch;
		ch = fgetc(fp); // get next symbol
	}

	getch();
	endwin();
	fclose(fp);

	return 0;
}