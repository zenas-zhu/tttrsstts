#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>
#include "updates.h"
#include "inputs.h"
#include "game.h"
#include "piece.h"

void draw_board(WINDOW *win, Updates *u);
void draw_queue(WINDOW *qwin, Updates *u);
void poll_kbd(WINDOW *win, Inputs *p);

int main()
{
	srand(time(NULL));
	initscr();
	// raw();
	cbreak();
	noecho();
	curs_set(0);
	WINDOW *win = newwin(22, 22, 0, 0);
	WINDOW *action = newwin(1, 22, 22, 0);
	WINDOW *qwin = newwin(20, 8, 1, 23);
	keypad(win, TRUE);
	box(win, 0, 0);
	Updates *u = updates_create();
	Inputs *p = inputs_create();
	Game *g = game_create();
	bool result = game_init(g, u);
	while (result) {
		draw_board(win, u);
		draw_queue(qwin, u);
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
	static char *draw_texts[4] = {"  ", "##", "##", "[]"};
	int *board = updates_get_board(u);
	for (int i = 0; i < 200; i++) {
		if (i % 10 == 0) {
			wmove(win, 20 - i / 10, 1);
		}
		wprintw(win, draw_texts[board[i]]);
	}
	wrefresh(win);
	wtimeout(win, updates_get_timeout(u));
}

void draw_queue(WINDOW *qwin, Updates *u)
{
	static char *draw_texts[2] = {"  ", "##"};
	static int i = 0;
	int *queue = updates_get_queue(u);
	if (updates_poll_redraw(u)) {
		werase(qwin);
		for (int i = 0; i < 5; i++) {
			int size = PIECE_SIZES[queue[i]];
			for (int j = 0; j < size * size; j++) {
				if (j % size == 0) {
					wmove(qwin, i * 4 + j / size, 0);
				}
				wprintw(qwin, draw_texts[PIECES[queue[i]][j]]);
			}
		}
		wrefresh(qwin);
		i++;
	}
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
