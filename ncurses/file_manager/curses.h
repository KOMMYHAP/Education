#ifndef CURSES_H
#define CURSES_H

#include <ncurses.h>

typedef enum {
	COLOR_BORDER = 0,
	COLOR_SELECTED,
	COLOR_TEXT
} color_t;

// инициализация curses
void init_curses();

#endif