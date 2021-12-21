#ifndef UPDATES_H
#define UPDATES_H

#include <stdbool.h>

/*
 * actions that the client will be required to perform
 */
typedef struct updates_ Updates;

/*
 * create an update packet
 */
Updates *updates_create();

/*
 * destroy an update packet
 */
void updates_destroy(Updates *updates);

/*
 * record the handle to the board state
 */
void updates_set_board(Updates *updates, int *board);

/*
 * retrieve the handle to the board state
 */
int *updates_get_board(Updates *updates);

/*
 * set max amount of time (ms) before the game should be polled again
 */
void updates_set_timeout(Updates *updates, long timeout);

/*
 * get max amount of time (ms) before polling the game again
 */
long updates_get_timeout(Updates *updates);

#endif
