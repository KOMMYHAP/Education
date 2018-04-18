#ifndef DETAIL_H
#define DETAIL_H

#include "curses.h"

#include <stdlib.h>

/* 	@brief: структура, для удобного хранения и передачи 
		окон
*/
typedef struct {
	WINDOW *data; // окно для записи текста
	WINDOW *border; // окно для обрамления
} window_t;


/* 	@brief: инициализация окон, расположенных в одну линию
	@params: 
		windows: массив указателей на window_t;
		n: кол-во окон;
		coef[n]: массив коэффициентов ширины данных окон; 
		h: высота окон,
		offset_y: смещение по y 
*/
void init_windows(window_t **window_ptrs, int n, float const coef[n], int h, int offset_y);


/* 	@brief: высвобождает все ресурсы, связанные с данным окном */
void window_free(window_t *window_ptr);


/* 	@brief: обертка над стандартной функцией waddstr с 
		особенностью автоматического отсечения невмещающихся
		символов и добавления \n в конец строки
	@params:
		window: окно, куда производится запись;
		message: сообщение, которое необходимо записать;
	@note: Явно указывать '\n' в message не требуется.
*/
void writeln(window_t *window_ptr, char const *message);

#endif