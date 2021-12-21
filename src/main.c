#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include "updates.h"
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
	Game *g = game_create();
	bool result = game_tick(g, -1, u);
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
		int key_raw = wgetch(win);
		int key = -1; // TODO: unknown keys treated as key timeout
		switch (key_raw) {
			case KEY_DOWN: key = 0; break;
			case KEY_UP: key = 1; break;
			case KEY_LEFT: key = 2; break;
			case KEY_RIGHT: key = 3; break;
			case 'o': case 'u': key = 4; break;
		}
		result = game_tick(g, key, u);
	}
	game_destroy(g);
	updates_destroy(u);
	endwin();
}
