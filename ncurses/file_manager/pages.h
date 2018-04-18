#ifndef PAGES_H
#define PAGES_H

#include "window_detail.h"
#include "content.h"

typedef struct _page_t
{
	window_t *window_ptr;
	content_t *content_ptr;
} page_t;

/*	@brief: инициализирует массив страниц */
page_t *init_pages(const char *path);


/*	@brief: высвобождает все ресурсы, связанные с page_ptrs
	@param:
		page_ptrs: массив page_t
		n: количество элементов в массиве */
void free_pages(page_t *pages_ptr);

#endif