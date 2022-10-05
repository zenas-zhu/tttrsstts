#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>
#include <unistd.h>
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
	wtimeout(win, 0);
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
		wprintw(action, "%s", u->action);
		wrefresh(action);
		wrefresh(win);
		usleep(16667);
		poll_kbd(win, p);
		result = game_tick(g, p, u);
	}
	wclear(action);
	draw_board(win, u);
	game_destroy(g);
	updates_destroy(u);
	inputs_destroy(p);
	{
		WINDOW *gameover = newwin(3, 16, 9, 11);
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
	int *board = u->board;
	for (int i = 0; i < 200; i++) {
		if (i % 10 == 0) {
			wmove(win, 20 - i / 10, 1);
		}
		if (board[i]) {
			int color = board[i] >= 2
			            ? board[i] - 1
			            : u->curcolor + 2;
			wattr_set(win, A_NORMAL, color, NULL);
			wprintw(win, "  ");
			wattr_set(win, A_NORMAL, 0, NULL);
		} else if (board[i] == 0) {
			wprintw(win, "  ");
		}
	}
	// static int i = 0;
	// wmove(win, 0, 0);
	// wprintw(win, "%d", i++);
}

void draw_hold(WINDOW *hwin, Updates *u)
{
	int hold = u->hold;
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
	int *queue = u->queue;
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
	p->keys = 0;
	int key_raw = wgetch(win);
	while (key_raw != -1) {
		int key_index;
		switch (key_raw) {
			case KEY_LEFT: key_index = GAME_KEY_LEFT; break;
			case KEY_RIGHT: key_index = GAME_KEY_RIGHT; break;
			case KEY_DOWN: key_index = GAME_KEY_SOFT_DROP; break;
			case KEY_UP: key_index = GAME_KEY_HARD_DROP; break;
			case 'f': key_index = GAME_KEY_CW; break;
			case 's': key_index = GAME_KEY_CCW; break;
			case 'd': key_index = GAME_KEY_180; break;
			case 'a': key_index = GAME_KEY_HOLD; break;
			default: continue;
		}
		p->keys |= 1 << key_index;
		key_raw = wgetch(win);
	}
}
