#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>
#include "updates.h"
#include "inputs.h"
#include "game.h"
#include "piece.h"

void draw_board(WINDOW *win, Updates *u);
void draw_hold(WINDOW *hwin, Updates *u);
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
	WINDOW *win = newwin(22, 22, 0, 8);
	WINDOW *hwin = newwin(4, 8, 1, 0);
	WINDOW *action = newwin(1, 22, 22, 8);
	WINDOW *qwin = newwin(20, 8, 1, 31);
	{
		start_color();
		use_default_colors();
		init_pair(0, -1, -1);
		init_pair(1, -1, 240);
		for (int i = 0; i < PIECES_SIZE; i++) {
			init_pair(2 + i, -1, PIECE_COLORS[i]);
		}
	}
	keypad(win, TRUE);
	box(win, 0, 0);
	Updates *u = updates_create();
	Inputs *p = inputs_create();
	Game *g = game_create();
	bool result = game_init(g, u);
	while (result) {
		draw_board(win, u);
		if (updates_poll_redraw(u)) {
			draw_hold(hwin, u);
			draw_queue(qwin, u);
		}
		wclear(action);
		wprintw(action, "%s", updates_get_action(u));
		wrefresh(action);
		wrefresh(win);
		wtimeout(win, updates_get_timeout(u));
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
		if (i % 10 == 0) {
			wmove(win, 20 - i / 10, 1);
		}
		if (board[i]) {
			int color = board[i] >= 2
			            ? board[i] - 1
			            : updates_get_curcolor(u) + 2;
			wattr_set(win, A_NORMAL, color, NULL);
			wprintw(win, "  ");
			wattr_set(win, A_NORMAL, 0, NULL);
		} else if (board[i] == 0) {
			wprintw(win, "  ");
		}
	}
}

void draw_hold(WINDOW *hwin, Updates *u)
{
	int hold = updates_get_hold(u);
	werase(hwin);
	if (hold >= 0) {
		wattr_set(hwin, A_NORMAL, 2 + hold, NULL);
		int size = PIECE_SIZES[hold];
		int offsetx = (4 - size) / 2 * 2;
		int offsety = (size - 3) / 2;
		for (int j = 0; j < size * size; j++) {
			if (PIECES[hold][j]) {
				wmove(hwin, j / size + offsety, j % size * 2 + offsetx);
				wprintw(hwin, "  ");
			}
		}
		wattr_set(hwin, A_NORMAL, 0, NULL);
	}
	wrefresh(hwin);
}

void draw_queue(WINDOW *qwin, Updates *u)
{
	int *queue = updates_get_queue(u);
	werase(qwin);
	for (int i = 0; i < 5; i++) {
		wattr_set(qwin, A_NORMAL, 2 + queue[i], NULL);
		int size = PIECE_SIZES[queue[i]];
		int offsetx = (4 - size) / 2 * 2;
		int offsety = (size - 3) / 2;
		for (int j = 0; j < size * size; j++) {
			if (PIECES[queue[i]][j]) {
				wmove(qwin, i * 4 + j / size + offsety, j % size * 2 + offsetx);
				wprintw(qwin, "  ");
			}
		}
		wattr_set(qwin, A_NORMAL, 0, NULL);
	}
	wrefresh(qwin);
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
		case 'a': a = ACTION_HOLD; break;
	}
	inputs_set_action(p, a);
}
