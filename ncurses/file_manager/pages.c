#include "pages.h"

const int page_n = 3;

window_t *init_pages()
{
	const float page_coef[] = {0.15f, 0.4f, 0.45f};

	int max_w, max_h;
	getmaxyx(stdscr, max_h, max_w);

	window_t *pages = (window_t *)malloc(sizeof(window_t) * page_n);
	if (pages == NULL) {
		return NULL;
	}

	init_windows(pages, page_n, page_coef, max_h - 3, 0);

	return pages;
}