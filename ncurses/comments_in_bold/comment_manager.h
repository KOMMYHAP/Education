#ifndef COMMENT_MANAGER_H
#define COMMENT_MANAGER_H

#ifndef TRUE
#define TRUE 1

#ifndef FALSE
#define FALSE 0

#define CLR_COMMENT 1
#define CLR_DEFAULT 2

#define MNG_DEFAULT 0
#define MNG_LINE 	1
#define MNG_MLINE 	2
#define MNG_QUOTE 	4

#define BRD_START_CMT 	1
#define BRD_END_CMT		2

typedef char mask_t;

/*
	
*/
void is_start_comment(mask_t);

/*
	
*/
void set_start_comment(int, int);

/*

*/
void is_end_comment(mas);

/*

*/
void set_end_comment(int, int);

/*

*/
void get_current_mask();

/*

*/
void initcmt();

/*

*/
int need_next_page();

/*
*/
void set_next_page();

#endif //COMMENT_MANAGER_H