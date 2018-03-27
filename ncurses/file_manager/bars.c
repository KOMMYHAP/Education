#include "bars.h"

const int bar_n  = 2;

window_t *init_bars()
{
	const float bar_coef[] = {0.8f, 0.2f};

	int max_w, max_h;
	getmaxyx(stdscr, max_h, max_w);

	window_t *bars = (window_t *)malloc(sizeof(window_t) * bar_n);
	if (bars == NULL) {
		return NULL;
	}

	init_windows(bars, bar_n, bar_coef, 3, max_h - 3);

	return bars;
}