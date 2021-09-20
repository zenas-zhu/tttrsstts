#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include "field.h"
#include "updates.h"

void do_updates(Updates *updates, WINDOW *win);

int main()
{
	srand(0);
	initscr();
	// raw();
	noecho();
	curs_set(0);
	keypad(stdscr, TRUE);
	WINDOW *win = newwin(22, 24, 0, 0);
	box(win, 0, 0);
	Updates *u = updates_create();
	Field *f = field_create(u);
	usleep(100000);
	Step_result result = field_step(f, STEP_TYPE_APPEAR, u);
	do_updates(u, win);
	while (result != STEP_RESULT_GAMEOVER) {
		int x = rand() % 10;
		if (x > 8) {
			usleep(100000);
			result = field_step(f, STEP_TYPE_LOCK, u);
			do_updates(u, win);
			usleep(500000);
			result = field_step(f, STEP_TYPE_APPEAR, u);
			do_updates(u, win);
		} else if (x < 2) {
			usleep(50000);
			result = field_step(f, x ? STEP_TYPE_LEFT : STEP_TYPE_RIGHT, u);
			do_updates(u, win);
			usleep(50000);
			result = field_step(f, STEP_TYPE_DOWN, u);
			do_updates(u, win);
		} else {
			usleep(100000);
			result = field_step(f, STEP_TYPE_DOWN, u);
			do_updates(u, win);
		}
		if (result == STEP_RESULT_LANDED) {
			usleep(500000);
			result = field_step(f, STEP_TYPE_LOCK, u);
			do_updates(u, win);
			usleep(500000);
			result = field_step(f, STEP_TYPE_APPEAR, u);
			do_updates(u, win);
		}
	}
	field_destroy(f);
	updates_destroy(u);
	endwin();
}

void do_updates(Updates *updates, WINDOW *win)
{
	while (updates_available(updates)) {
		Update u = updates_remove(updates);
		if (u.update_type == UPDATE_DRAW_CELL) {
			char *draw_texts[3] = {"  ", "[]", "##"};
			char *draw_text = draw_texts[u.draw_cell_color];
			mvwprintw(win, 20 - u.draw_cell_r, u.draw_cell_c * 2 + 2, draw_text);
		}
	}
	wrefresh(win);
}
