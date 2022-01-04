#include <stdlib.h>
#include "updates.h"

struct updates_ {
	int *board;
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
