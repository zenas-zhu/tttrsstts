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
 * record the hold piece
 */
void updates_set_hold(Updates *updates, int hold);

/*
 * retrieve the hold piece
 */
int updates_get_hold(Updates *updates);

/*
 * record the handle to the piece queue
 */
void updates_set_queue(Updates *updates, int *queue);

/*
 * retrieve the handle to the piece queue
 */
int *updates_get_queue(Updates *updates);

/*
 * record the active piece color
 */
void updates_set_curcolor(Updates *updates, int curcolor);

/*
 * retrieve the active piece color
 */
int updates_get_curcolor(Updates *updates);

/*
 * flag a redraw of the hold piece and piece queue
 */
void updates_flag_redraw(Updates *updates);

/*
 * poll and clear the hold/queue redraw flag
 */
bool updates_poll_redraw(Updates *updates);

/*
 * set max amount of time (ms) before the game should be polled again
 */
void updates_set_timeout(Updates *updates, long timeout);

/*
 * get max amount of time (ms) before polling the game again
 */
long updates_get_timeout(Updates *updates);

/*
 * set action text after line clear (e.g. "single")
 */
void updates_set_action(Updates *updates, char *action);

/*
 * get action text after line clear (e.g. "single")
 */
char *updates_get_action(Updates *updates);

#endif
