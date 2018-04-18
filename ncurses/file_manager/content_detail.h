#ifndef CONTENT_DETAIL_H
#define CONTENT_DETAIL_H

#include <unistd.h> /* read */
#include <fcntl.h> 
#include <stdio.h> /* dprintf */
#include <string.h> /* memset, strcmp, strlen */
#include <stdlib.h> /* malloc */
#include <errno.h>

/* 	@brief: Считывает по заданному файловому дескриптору 
	строку вплоть до '\n' или EOF
	@return: 
		in EOF case, zero is returned,
		in '\n' case, one is returned,
		in other case, -1 is returned and errno is setted.
*/
int readln(int fd, char* *message_ptr);


/*	@brief: Checks equility path and "/" 
	@return: if true  1 is returned and
			 if false 0 is returned */
int is_root(const char *path);


/*	@brief: Выходит на один уровень выше, относительно
		директории, указанной в path
	@param:
		path: абсолютный путь к текущей директории, 
		к примеру, "/home/kommyhap"
	@note: path не должен оканчиваться на "/"
	@return: 
		Если переход выполнен успешно, то 
		возвращается соответствующий путь,
		в ином случае будет возвращено NULL */
char *get_previous_dir(const char *path);


/*	@brief: Переходит на один уровень ниже, относительно
		директории, указанной в path в директорию dir
	@param:
		path: абсолютный путь к текущей директории, 
		к примеру, "/home/kommyhap"
		dir: директорию, в которую будет совершен переход,
		к примеру, "programs"
	@note: 
		path не должен оканчиваться на "/",
		dir не должен иметь "/"
	@return: 
		Если переход выполнен успешно, то 
		возвращается соответствующий путь,
		в ином случае будет возвращено NULL */
char *get_next_dir(const char *path, const char *dir);

#endif
