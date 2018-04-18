#ifndef LOGFILE_H
#define LOGFILE_H

#include <stdio.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/*	@brief: создает в текущей директории read-onle файл */
int logfile_create(const char *filename);


/* 	@brief: заменяет дескриптор STDERR_FILENO на
	дескриптор файла и именем filename
	@return:
		if success case fd of logfile,
		-1 in otherwise */
int logfile_init(const char *filename);

#endif
