#ifndef BARS_H
#define BARS_H

#include "detail.h"

extern const int bar_n;

typedef enum { /* bars */
	message = 0,
	status
} bar_t;

/*	@brief: инициализирует массив панелей */
window_t *init_bars();

#endif