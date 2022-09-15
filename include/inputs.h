#ifndef INPUTS_H
#define INPUTS_H

#include <stdbool.h>

/*
 * enumeration of keys that can be pressed
 * these should correspond 1-1 to physical keys on a keyboard, but do not care about control schemes or key maps
 */
typedef enum {
	GAME_KEY_LEFT,
	GAME_KEY_RIGHT,
	GAME_KEY_SOFT_DROP,
	GAME_KEY_HARD_DROP,
	GAME_KEY_CW,
	GAME_KEY_CCW,
	GAME_KEY_180,
	GAME_KEY_HOLD,
} GameKey;

/*
 * the set of keys currently being presssed
 */
typedef struct {
	int keys; // bitmap of keys pressed this tick
} Inputs;

/*
 * create an input packet
 */
Inputs *inputs_create();

/*
 * destroy an input packet
 */
void inputs_destroy(Inputs *inputs);

#endif
