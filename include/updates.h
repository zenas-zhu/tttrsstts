#ifndef UPDATES_H
#define UPDATES_H

#include <stdbool.h>

/*
 * actions that the client will be required to perform
 */
typedef struct {
	enum {
		UPDATE_NONE,
		UPDATE_DRAW_CELL,
		// UPDATE_GET_KEY,
	} update_type;
	union {
		struct {
			int draw_cell_r;
			int draw_cell_c;
			int draw_cell_color;
		};
	};
} Update;

typedef struct updates_ Updates;

/*
 * create an update queue
 */
Updates *updates_create();

/*
 * destroy an update queue
 */
void updates_destroy(Updates *updates);

/*
 * add an update to the queue
 */
void updates_add(Updates *updates, Update u);

/*
 * remove an update from the queue
 */
Update updates_remove(Updates *updates);

/*
 * is there an update in the queue?
 */
bool updates_available(Updates *updates);

#endif
