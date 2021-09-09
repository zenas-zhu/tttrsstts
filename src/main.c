#include <stdio.h>
#include <stdlib.h>
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
	Field *f = field_create();
	usleep(100000);
	Step_result result = field_step(f, STEP_TYPE_DOWN);
	while (result != STEP_RESULT_GAMEOVER) {
		int x = rand() % 10;
		if (x > 8) {
			usleep(100000);
			result = field_step(f, STEP_TYPE_LOCK);
			usleep(500000);
			result = field_step(f, STEP_TYPE_APPEAR);
		} else if (x < 2) {
			usleep(50000);
			result = field_step(f, x ? STEP_TYPE_LEFT : STEP_TYPE_RIGHT);
			usleep(50000);
			result = field_step(f, STEP_TYPE_DOWN);
		} else {
			usleep(100000);
			result = field_step(f, STEP_TYPE_DOWN);
		}
		if (result == STEP_RESULT_LANDED) {
			usleep(500000);
			result = field_step(f, STEP_TYPE_LOCK);
			usleep(500000);
			result = field_step(f, STEP_TYPE_APPEAR);
		}
	}
	field_destroy(f);
	endwin();
}
