#include "curses.h"

void init_curses()
{
    initscr();
    start_color();
    init_pair(COLOR_TEXT, COLOR_WHITE, COLOR_BLACK);

	wbkgd(stdscr, COLOR_PAIR(COLOR_TEXT));

    curs_set(0); // невидимый курсор
    noecho();
    keypad(stdscr, TRUE);
}