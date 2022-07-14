#include <stdlib.h>
#include "updates.h"

Updates *updates_create()
{
	Updates *updates = malloc(sizeof(Updates));
	return updates;
}

void updates_destroy(Updates *updates)
{
	free(updates);
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
