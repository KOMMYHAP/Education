#include <ncurses.h>

int main()
{
	initscr();

	raw(); // line buffering disabled
	keypad(stdscr, TRUE); // wt get f1, f2.., arrows, etc.
	noecho(); // echo disabled

	printw("Type any character to see it in bold\n");
	int ch = getch();

	if (ch == KEY_F(1)) {
		printw("F1 key pressed");
	}

	printw("The pressed key is ");
	attron(A_BOLD);
	printw("%c", ch);
	attroff(A_BOLD);

	refresh();

	getch();
	endwin();
	return 0;
}