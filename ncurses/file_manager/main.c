#include <errno.h>

#include "curses.h"
#include "pages.h"
#include "bars.h"

int main(int argc, char *argv[])
{
	window_t *pages = NULL, *bars = NULL;

	init_curses();

	pages = init_pages();
	if (pages == NULL) {
		errno = ENOSPC;
		goto free_label;
	}

	bars = init_bars();
	if (bars == NULL) {
		errno = ENOSPC;
		goto free_label;
	}

    writeln(pages[prev], "kommyhap");

    writeln(pages[cur ], "programs");

    writeln(pages[next], "education");

    writeln(bars[message], "Message bar testing.");
    writeln(bars[status ], "100%");

    getch();

free_label:
    destroy_windows(pages, page_n);
    destroy_windows(bars, bar_n);

	endwin();

	if (errno != 0) {
		perror("\033[0;31mE\033[0m");
	}

	return 0;
}