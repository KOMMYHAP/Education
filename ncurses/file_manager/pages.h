#ifndef PAGES_H
#define PAGES_H

#include "detail.h"

extern const int page_n;

typedef enum { /* pages */
	prev = 0, 
	cur, 
	next
} page_t;

/*	@brief: инициализирует массив страниц */
window_t *init_pages();

#endif