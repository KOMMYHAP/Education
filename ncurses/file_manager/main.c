#include <ncurses.h>
#include <assert.h>
#include <stdlib.h>

const float page_coef[] = {0.15f, 0.4f, 0.45f};
const int page_n = 3;

const float bar_coef[] = {0.8f, 0.2f};
const int bar_n  = 2;

typedef enum { /* pages */
	prev = 0, 
	cur, 
	next
} page_t;

typedef enum { /* bars */
	message = 0,
	status
} bar_t;

typedef enum {
	COLOR_BORDER = 0,
	COLOR_SELECTED,
	COLOR_TEXT
} color_t;


typedef struct {
	WINDOW *data;
	WINDOW *border;
} window_t;

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

void init_windows(window_t *windows, int n, float const coef[n], int h, int offset_y)
{
	int max_w, max_h;
	getmaxyx(stdscr, max_h, max_w);	

	int offset_x = 0, w;
	for (int i = 0; i < n; ++i) {
		w = (int)(max_w * coef[i]);

		windows[i].border = /* params: (h, w, y, x) */
			subwin(stdscr, h, w, offset_y, offset_x);
		box(windows[i].border, ACS_VLINE, ACS_HLINE);

		windows[i].data =
			subwin(windows[i].border, h - 2, w - 2, offset_y + 1, offset_x + 1);

		offset_x += w;
	}
}

void init_pages(window_t *pages)
{
	int max_w, max_h;
	getmaxyx(stdscr, max_h, max_w);

	init_windows(pages, page_n, page_coef, max_h - 3, 0);
}

void init_bars(window_t *bars)
{
	int max_w, max_h;
	getmaxyx(stdscr, max_h, max_w);

	init_windows(bars, bar_n, bar_coef, 3, max_h - 3);
}

void destroy_windows(window_t *windows, int n) 
{
    for (int i = 0; i < n; ++i) {
    	delwin(windows[i].data);
    	delwin(windows[i].border);
    }
}

int main()
{
	window_t pages[page_n];
	window_t bars[bar_n];

	init_curses();

	init_pages(pages);
	init_bars(bars);

    waddstr(pages[prev].data, "kommyhap\n");

    waddstr(pages[cur ].data, "programs\n");

    waddstr(pages[next].data, "education\n");

    waddstr(bars[message].data, "Message bar testing.\n");
    waddstr(bars[status ].data, "100%\n");

    getch();

    destroy_windows(pages, page_n);
    destroy_windows(bars, bar_n);

	endwin();

	return 0;
}