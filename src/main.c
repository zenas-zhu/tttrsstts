#include <stdio.h>
#include <unistd.h>
#include <ncurses.h>
#include "field.h"

int main()
{
	initscr();
	// raw();
	noecho();
	curs_set(0);
	keypad(stdscr, TRUE);
	field_t *f = create_field();
	while (step_field(f)) {
		usleep(100000);
	}
	wgetch(f->view);
	destroy_field(f);
	endwin();
}
