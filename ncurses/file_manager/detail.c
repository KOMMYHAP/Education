#include "detail.h"

#include <string.h> /* strlen */

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

void destroy_windows(window_t *windows, int n) 
{
	if (windows == NULL) {
		return;
	}
	
    for (int i = 0; i < n; ++i) {
    	delwin(windows[i].data);
    	delwin(windows[i].border);
    }
    free(windows);
}

void writeln(window_t window, char const *message)
{
	char filler[] = "..";

	int max_h, max_w;
	getmaxyx(window.data, max_h, max_w);

	int y, x;
	getyx(window.data, y, x);

	wprintw(window.data, "%.*s", max_w, message);

	if (strlen(message) > max_w + 1) { // "\n" is nessessary to consider
		x = max_w - strlen(filler) - 1;
		if (x < 0) {
			x = 0;
		}

		mvwprintw(window.data, y, x, "%s", filler);
	}

	x = max_w - 1;
	if (x < 0) {
		x = 0;
	}
	mvwprintw(window.data, y, x, "\n");
}