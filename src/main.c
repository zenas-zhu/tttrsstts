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
	while (result) {
		updates_do_draw(u, drawer, win);
		wrefresh(win);
		// wtimeout(win, updates_get_timeout(u));
		int key_raw = wgetch(win);
		int key = -1; // TODO: unknown keys treated as key timeout
		switch (key_raw) {
			case KEY_DOWN: key = 0; break;
			case KEY_UP: key = 1; break;
			case KEY_LEFT: key = 2; break;
			case KEY_RIGHT: key = 3; break;
		}
		result = game_tick(g, key, u);
	}
	game_destroy(g);
	updates_destroy(u);
	endwin();
}

void drawer(void *ctx, int r, int c, int color)
{
	char *draw_texts[3] = {"  ", "[]", "##"};
	char *draw_text = draw_texts[color];
	mvwprintw((WINDOW *)ctx, 20 - r, c * 2 + 2, draw_text);
}
