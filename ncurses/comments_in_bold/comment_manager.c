static char one_line[][] = {
	"//"
};

static char multiple_line[][] = {
	"/*", "*/"
};

static mask_t cmt_manager;

int is_start_comment(mask_t mask)
{
	return mask & BRD_START_CMT == BRD_START_CMT
}

void set_start_comment()
{
	attron(A_BOLD | COLOR_PAIR(CLR_COMMENT));
	move(y, x - 1);
	printw("/*");
}

void end_comment(int x, int y)
{
	move(y, x - 1);
	printw("*/");	
	attroff(A_BOLD | COLOR_PAIR(CLR_COMMENT));
}

mask_t get_current_mask()
{
	int x, y;
	getyx(stdscr, y, x);

	int ch 		= mvgetc(y, x - 1),
		prev 	= mvgetc(y, x - 2);

	if (prev != '\\' && ch == '"') {
		cmt_manager.flags ~= MNG_QUOTE;
		return cmt_manager;
	}



}

void initcmt()
{
	start_color();
	init_pair(CLR_COMMENT, COLOR_GREEN, COLOR_BLACK);
	init_pair(CLR_DEFAULT, COLOR_WHITE, COLOR_BLACK);
	cmt_manager.flags &= MNG_DEFAULT;
}

int need_next_page()
{
	int x, y, row, col;
	getmaxyx(row, col);
	getyx(y, x);
	return y == row - 1;
}

void set_next_page()
{
	printw("<----Press any key---->");
	getch();
	clear();
	move(0, 0);
}