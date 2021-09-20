#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include "field.h"
#include "updates.h"

void drawer(void *ctx, int r, int c, int color);

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
	updates_do_draw(u, drawer, win);
	while (result != STEP_RESULT_GAMEOVER) {
		int x = rand() % 10;
		if (x > 8) {
			usleep(100000);
			result = field_step(f, STEP_TYPE_LOCK, u);
			updates_do_draw(u, drawer, win);
			usleep(500000);
			result = field_step(f, STEP_TYPE_APPEAR, u);
			updates_do_draw(u, drawer, win);
		} else if (x < 2) {
			usleep(50000);
			result = field_step(f, x ? STEP_TYPE_LEFT : STEP_TYPE_RIGHT, u);
			updates_do_draw(u, drawer, win);
			usleep(50000);
			result = field_step(f, STEP_TYPE_DOWN, u);
			updates_do_draw(u, drawer, win);
		} else {
			usleep(100000);
			result = field_step(f, STEP_TYPE_DOWN, u);
			updates_do_draw(u, drawer, win);
		}
		if (result == STEP_RESULT_LANDED) {
			usleep(500000);
			result = field_step(f, STEP_TYPE_LOCK, u);
			updates_do_draw(u, drawer, win);
			usleep(500000);
			result = field_step(f, STEP_TYPE_APPEAR, u);
			updates_do_draw(u, drawer, win);
		}
	}
	field_destroy(f);
	updates_destroy(u);
	endwin();
}

void drawer(void *ctx, int r, int c, int color)
{
	char *draw_texts[3] = {"  ", "[]", "##"};
	char *draw_text = draw_texts[color];
	mvwprintw((WINDOW *)ctx, 20 - r, c * 2 + 2, draw_text);
	wrefresh((WINDOW *)ctx); // too many refreshes ik
}
