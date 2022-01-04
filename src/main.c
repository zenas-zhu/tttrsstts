#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include "updates.h"
#include "inputs.h"
#include "game.h"

void draw_board(WINDOW *win, Updates *u);
void poll_kbd(WINDOW *win, Inputs *p);

int main()
{
	initscr();
	// raw();
	cbreak();
	noecho();
	curs_set(0);
	WINDOW *win = newwin(22, 22, 0, 0);
	WINDOW *action = newwin(1, 22, 22, 0);
	keypad(win, TRUE);
	box(win, 0, 0);
	Updates *u = updates_create();
	Inputs *p = inputs_create();
	Game *g = game_create();
	bool result = game_tick(g, NULL, u);
	while (result) {
		draw_board(win, u);
		wclear(action);
		wprintw(action, "%s", updates_get_action(u));
		wrefresh(action);
		poll_kbd(win, p);
		result = game_tick(g, p, u);
	}
	wclear(action);
	draw_board(win, u);
	game_destroy(g);
	updates_destroy(u);
	inputs_destroy(p);
	{
		WINDOW *gameover = subwin(win, 3, 16, 9, 3);
		box(gameover, 0, 0);
		mvwprintw(gameover, 1, 2, "game over :(");
		wgetch(gameover);
		delwin(gameover);
	}
	delwin(win);
	endwin();
}

void draw_board(WINDOW *win, Updates *u)
{
	int *board = updates_get_board(u);
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
}

void poll_kbd(WINDOW *win, Inputs *p)
{
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
}
