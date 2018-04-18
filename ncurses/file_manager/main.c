#include "logfile.h"

#include "curses.h"
#include "pages.h"

int main(int argc, char *argv[])
{
	page_t *pages;
	int retvalue, logfile_fd;

	logfile_fd = logfile_init("logfile");
	if (logfile_fd == -1) {
		return -1;
	}

	init_curses();

	pages = init_pages("/home/kommyhap/");

    getch();

	endwin();

	return 0;
}