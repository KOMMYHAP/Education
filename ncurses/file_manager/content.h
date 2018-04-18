#ifndef CONTENT_H
#define CONTENT_H

#include "content_detail.h"

#include <sys/types.h> 	/* waitpid */
#include <sys/wait.h>	/* waitpid */

#include <stdio.h> /* perror */

#include <limits.h> /* INT_MAX */


/* @brief:	содержимое страницы, а именно
		1. Массив строк
		2. Размер массива (в строках)
		3. Смещение от начала массива */
typedef struct _content_t
{
	char* *strings;
	int size;
	int capacity;
	int offset;
} content_t;


/* 	@brief: инициализация контента таким образом, 
	что: 
		Первый элемент в списке будет содержать
		информацию о предыдущей директории;
		Второй - о текущей директории;
		Третий - о первой директории во втором контенте; 
	@param:
		content_ptrs: массив указателей на content_t,
		n: количество указателей в массиве,
		path: путь до текущей директории 
	@return:
		is success case 0 is returned,
		-1 in otherwise */
int init_contents(content_t **content_ptrs, int n, char const  *path);


/* 	@brief: инициализирует поля структуры значениями
	по умолчанию
	@return:
		in success case, zero is returned and
		-1 otherwise and errno is setted */
int content_init(content_t *content_ptr);


/* @brief: загружает содержимое директории, указанной в path,
	в content_ptr
	@return:
		in success case, zero is returned and
		-1 otherwise and errno is setted */
int content_load(const char *path, content_t *content_ptr);


/* 	@brief: возвращает конкретную строку по индексу index
	@return:
		in success case, existing string is returned and
		NULL otherwise  */
char const* content_get(content_t const *content_cptr, int index);


/* 	@brief: перемещает сообщение в конец массива
	@return: 
		in success case, zero is returned and -1 otherwise */
int content_move(content_t *content_ptr, char *msg);


/* @brief: высвобождает выделенную под content память */
void content_free(content_t *content_ptr);

#endif