#include <stdlib.h>
#include <stddef.h>
#include "updates.h"

#define UPDATES_SIZE 256

struct updates_ {
	size_t i0; // start
	size_t n; // size
	Update *v;
};

// TODO: this is incorrect in the case of many updates

Updates *updates_create()
{
	Updates *updates = malloc(sizeof(Updates));
	updates->i0 = 0;
	updates->n = 0;
	updates->v = malloc(sizeof(Update) * UPDATES_SIZE);
	return updates;
}

void updates_destroy(Updates *updates)
{
	free(updates->v);
	free(updates);
}

void updates_add(Updates *updates, Update u)
{
	updates->v[(updates->i0 + updates->n) % UPDATES_SIZE] = u;
	updates->n += 1;
}

Update updates_remove(Updates *updates)
{
	if (!updates_available(updates)) {
		return (Update){.update_type = UPDATE_NONE};
	}
	Update u = updates->v[updates->i0 % UPDATES_SIZE];
	updates->n -= 1;
	updates->i0 += 1;
	return u;
}

bool updates_available(Updates *updates)
{
	return updates->n > 0;
}
