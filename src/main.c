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
	usleep(100000);
	step_result_e result = step_field(f, STEP_TYPE_DOWN);
	while (result != STEP_RESULT_GAMEOVER) {
		int x = rand() % 10;
		if (x < 2) {
			usleep(50000);
			result = step_field(f, x ? STEP_TYPE_LEFT : STEP_TYPE_RIGHT);
			usleep(50000);
			result = step_field(f, STEP_TYPE_DOWN);
		} else {
			usleep(100000);
			result = step_field(f, STEP_TYPE_DOWN);
		}
	}
	wgetch(f->view);
	destroy_field(f);
	endwin();
}
