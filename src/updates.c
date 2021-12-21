#include <stdlib.h>
#include <stddef.h>
#include "updates.h"

#define UPDATES_SIZE 256

struct updates_ {
	int *board;
	long timeout;
};

// TODO: this is incorrect in the case of many updates

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
