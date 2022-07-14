#ifndef INPUTS_H
#define INPUTS_H

#include <time.h>

/*
 * the actions that can be called
 * these should correspond 1-1 to physical keys, but do not care about control schemes or key maps
 */
typedef enum {
	ACTION_NONE,
	ACTION_LEFT,
	ACTION_RIGHT,
	ACTION_SOFT_DROP,
	ACTION_HARD_DROP,
	ACTION_CW,
	ACTION_CCW,
	ACTION_180,
	ACTION_HOLD,
} Action;

/*
 * a keypress and when it occured
 */
typedef struct {
	Action action; // input action
	struct timespec base; // time when the game starts waiting for input
	struct timespec mark; // time when input is received
} Inputs;

/*
 * create an input packet
 */
Inputs *inputs_create();

/*
 * destroy an input packet
 */
void inputs_destroy(Inputs *inputs);

/*
 * set the input packet's "base" time to the current time
 */
void inputs_reset_millis(Inputs *inputs);

/*
 * remember the current time
 */
 void inputs_measure_millis(Inputs *inputs);

/*
 * get the difference (ms) between the time of the last measured time and the "base" time
 */
long inputs_get_millis(Inputs *inputs);

#endif
