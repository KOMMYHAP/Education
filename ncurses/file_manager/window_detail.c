#include "window_detail.h"

#include <string.h> /* strlen */

void init_windows(window_t **window_ptrs, int n, float const coef[n], int h, int offset_y)
{
	int max_w, max_h;
	getmaxyx(stdscr, max_h, max_w);	

	int offset_x = 0, w;
	for (int i = 0; i < n; ++i) {
		w = (int)(max_w * coef[i]);

		window_ptrs[i]->border = /* params: (h, w, y, x) */
			subwin(stdscr, h, w, offset_y, offset_x);
		box(window_ptrs[i]->border, ACS_VLINE, ACS_HLINE);

		window_ptrs[i]->data =
			subwin(window_ptrs[i]->border, h - 2, w - 2, offset_y + 1, offset_x + 1);

		offset_x += w;
	}
}

void window_free(window_t *window_ptr)
{
	if (window_ptr != NULL) {
		delwin(window_ptr->data);
		delwin(window_ptr->border);
		
	    free(window_ptr);
	}
}

void writeln(window_t *window_ptr, char const *message)
{
	char filler[] = "..";

	int max_h, max_w;
	getmaxyx(window_ptr->data, max_h, max_w);

	int y, x;
	getyx(window_ptr->data, y, x);

	wprintw(window_ptr->data, "%.*s", max_w, message);

	if (strlen(message) > max_w + 1) { // "\n" is nessessary to consider
		x = max_w - strlen(filler) - 1;
		if (x < 0) {
			x = 0;
		}

		mvwprintw(window_ptr->data, y, x, "%s", filler);
	}

	x = max_w - 1;
	if (x < 0) {
		x = 0;
	}
	mvwprintw(window_ptr->data, y, x, "\n");
}