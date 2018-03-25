#include <ncurses.h>
#include <stdlib.h>

#define ENTER 10
#define ESCAPE 27

void init_curses()
{
    initscr();
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    init_pair(2, COLOR_BLUE, COLOR_WHITE);
    init_pair(3, COLOR_RED, COLOR_WHITE);
    curs_set(0); // невидимый курсор
    noecho();
    keypad(stdscr, TRUE);
}



int main()
{
	enum {
		prev = 0, 
		cur, 
		next
	};

	WINDOW *page[3];
	WINDOW *message_bar, *status_bar;

	init_curses();
	bkgd(COLOR_PAIR(1));

	int max_w, max_h;
	getmaxyx(stdscr, max_h, max_w);

	int length[3]; /* 10% | 30% | 60% */
	length[prev] = (int)(max_w * 0.1f);
	length[cur ] = (int)(max_w * 0.3f);
	length[next] = (int)(max_w * 0.6f);

	page[prev] = 
		subwin(stdscr, max_h - 2, length[prev], 0, 0);

	page[cur ] = 
		subwin(stdscr, max_h - 2, length[cur], 0, length[prev] + 7);

	page[next] = 
		subwin(stdscr, max_h - 2, length[next], 0, length[prev] + length[cur]);


    waddstr(page[prev], "kommyhap");
    waddstr(page[cur ], "programs");
    waddstr(page[next], "education");

    getch();

    for (int i = 0; i < 3; ++i) {
    	delwin(page[i]);
    }

	endwin();
}