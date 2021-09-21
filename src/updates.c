#include <stdlib.h>
#include <stddef.h>
#include "updates.h"

#define UPDATES_SIZE 256

struct updates_ {
	struct {
		size_t n;
		int *v;
	} draw_cell;
	long timeout;
};

// TODO: this is incorrect in the case of many updates

Updates *updates_create()
{
	Updates *updates = malloc(sizeof(Updates));
	updates->draw_cell.n = 0;
	updates->draw_cell.v = malloc(sizeof(int) * 3 * UPDATES_SIZE);
	return updates;
}

void updates_destroy(Updates *updates)
{
	free(updates->draw_cell.v);
	free(updates);
}

void updates_queue_draw(Updates *updates, int r, int c, int color)
{
	size_t i = updates->draw_cell.n;
	updates->draw_cell.v[i * 3] = r;
	updates->draw_cell.v[i * 3 + 1] = c;
	updates->draw_cell.v[i * 3 + 2] = color;
	updates->draw_cell.n += 1;
}

void updates_do_draw(Updates *updates, void (*drawer)(void *ctx, int r, int c, int color), void *ctx)
{
	for (size_t i = 0; i < updates->draw_cell.n; i++) {
		drawer(ctx,
				updates->draw_cell.v[i * 3],
				updates->draw_cell.v[i * 3 + 1],
				updates->draw_cell.v[i * 3 + 2]);
	}
	updates->draw_cell.n = 0;
}

void updates_set_timeout(Updates *updates, long timeout)
{
	updates->timeout = timeout;
}

long updates_get_timeout(Updates *updates)
{
	return updates->timeout;
}
