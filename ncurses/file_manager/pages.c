#include "pages.h"

page_t *init_pages(const char *path)
{

	int max_w, max_h;
	getmaxyx(stdscr, max_h, max_w);

	window_t *window_ptrs[3];
	memset(window_ptrs, 0, sizeof(window_ptrs));
	for (int i = 0; i < 3; ++i) {
		window_ptrs[i] = (window_t*)malloc(sizeof(window_t));
		if (window_ptrs[i] == NULL) {
			errno = ENOMEM;
			perror("[init_pages: window_t allocation]");
			goto free_label_w;
		}
	}

	const float page_coef[] = {0.15f, 0.4f, 0.45f};
	init_windows(window_ptrs, 3, page_coef, max_h - 3, 0);

	content_t *content_ptrs[3];
	memset(content_ptrs, 0, sizeof(content_ptrs));
	for (int i = 0; i < 3; ++i) {
		content_ptrs[i] = (content_t*)malloc(sizeof(content_t));
		if (content_ptrs[i] == NULL) {
			errno = ENOMEM;
			perror("[init_pages: content_t allocation]");
			goto free_label_c;
		}
	}
	if (init_contents(content_ptrs, 3, path) != 0) {
		perror("[init_contents]");
		goto free_label_c;
	}

	page_t *pages = (page_t*)malloc(sizeof(page_t) * 3);
	if (pages == NULL) {
		goto free_label_p;
	}
	for (int i = 0; i < 3; ++i) {
		pages[i].window_ptr  = window_ptrs[i];
		pages[i].content_ptr = content_ptrs[i];
	}

	return pages;

free_label_p:
	errno = ENOMEM;
	perror("[pages allocation]");
	free(pages);

free_label_c:
	for (int i = 0; i < 3; ++i) {
		content_free(content_ptrs[i]);
	}

free_label_w:
	for (int i = 0; i < 3; ++i) {
		window_free(window_ptrs[i]);
	}

	return NULL;
}

void free_pages(page_t *pages_ptr)
{
	for (int i = 0; i < 3; ++i) {
		content_free(pages_ptr[i].content_ptr);
		window_free	(pages_ptr[i].window_ptr );
	}
	free(pages_ptr);
}