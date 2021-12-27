#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include "updates.h"
#include "inputs.h"
#include "game.h"

void drawer(void *ctx, int r, int c, int color);

int main()
{
	initscr();
	// raw();
	cbreak();
	noecho();
	curs_set(0);
	WINDOW *win = newwin(22, 24, 0, 0);
	keypad(win, TRUE);
	box(win, 0, 0);
	Updates *u = updates_create();
	Inputs *p = inputs_create();
	Game *g = game_create();
	bool result = game_tick(g, NULL, u);
	int *board = updates_get_board(u);
	while (result) {
		for (int i = 0; i < 200; i++) {
			static char *draw_texts[3] = {"  ", "##", "[]"};
			char *draw_text = draw_texts[board[i]];
			if (i % 10 == 0) {
				wmove(win, 20 - i / 10, 1);
			}
			wprintw(win, draw_text);
		}
		wrefresh(win);
		wtimeout(win, updates_get_timeout(u));
		inputs_reset_millis(p);
		int key_raw = wgetch(win);
		inputs_measure_millis(p);
		Action a = ACTION_NONE;
		switch (key_raw) {
			case KEY_DOWN: a = ACTION_SOFT_DROP; break;
			case KEY_UP: a = ACTION_HARD_DROP; break;
			case KEY_LEFT: a = ACTION_LEFT; break;
			case KEY_RIGHT: a = ACTION_RIGHT; break;
			case 'o': a = ACTION_CCW; break;
			case 'e': a = ACTION_180; break;
			case 'u': a = ACTION_CW; break;
		}
		inputs_set_action(p, a);
		result = game_tick(g, p, u);
	}
	game_destroy(g);
	updates_destroy(u);
	inputs_destroy(p);
	endwin();
}
