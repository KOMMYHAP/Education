#include "curses.h"
#include "pages.h"
#include "bars.h"

int main(int argc, char *argv[])
{
	window_t *pages = NULL, *bars = NULL;

	init_curses();

	pages = init_pages();
	if (pages == NULL) {
		fprintf(stderr, "E: init_pages() could not allocate the memmory.\n");
		goto free_label;
	}

	bars = init_bars();
	if (bars == NULL) {
		fprintf(stderr, "E: init_bars() could not allocate the memmory.\n");
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

	return 0;
}