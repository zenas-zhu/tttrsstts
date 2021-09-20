#ifndef UPDATES_H
#define UPDATES_H

#include <stdbool.h>

/*
 * actions that the client will be required to perform
 */
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
 * queues a draw cell update
 */
void updates_queue_draw(Updates *updates, int r, int c, int color);

/*
 * executes all the draw updates through the callback
 */
void updates_do_draw(Updates *updates, void (*drawer)(void *ctx, int r, int c, int color), void *ctx);

#endif
