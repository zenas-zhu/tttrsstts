#include <stdlib.h>
#include "updates.h"

struct updates_ {
	int *board;
	int hold;
	int *queue;
	int curcolor;
	bool redraw;
	long timeout;
	char *action;
};

Updates *updates_create()
{
	Updates *updates = malloc(sizeof(Updates));
	return updates;
}

void updates_destroy(Updates *updates)
{
	free(updates);
}

void updates_set_board(Updates *updates, int *board)
{
	updates->board = board;
}

int *updates_get_board(Updates *updates)
{
	return updates->board;
}

void updates_set_hold(Updates *updates, int hold)
{
	updates->hold = hold;
}

int updates_get_hold(Updates *updates)
{
	return updates->hold;
}

void updates_set_queue(Updates *updates, int *queue)
{
	updates->queue = queue;
}

int *updates_get_queue(Updates *updates)
{
	return updates->queue;
}

void updates_set_curcolor(Updates *updates, int curcolor)
{
	updates->curcolor = curcolor;
}

int updates_get_curcolor(Updates *updates)
{
	return updates->curcolor;
}

void updates_flag_redraw(Updates *updates)
{
	updates->redraw = true;
}

bool updates_poll_redraw(Updates *updates)
{
	bool redraw = updates->redraw;
	updates->redraw = false;
	return redraw;
}

void updates_set_timeout(Updates *updates, long timeout)
{
	updates->timeout = timeout;
}

long updates_get_timeout(Updates *updates)
{
	return updates->timeout;
}

void updates_set_action(Updates *updates, char *action)
{
	updates->action = action;
}

char *updates_get_action(Updates *updates)
{
	return updates->action;
}
