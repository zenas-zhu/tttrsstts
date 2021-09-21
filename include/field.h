#ifndef FIELD_H
#define FIELD_H

#include "updates.h"

typedef struct field_ Field;

/*
 * types of movements that can change a playfield
 */
typedef enum {
	STEP_TYPE_DOWN,
	STEP_TYPE_LEFT,
	STEP_TYPE_RIGHT,
	// STEP_TYPE_CW,
	// STEP_TYPE_CCW,
	STEP_TYPE_LOCK,
	// STEP_TYPE_CLEAR, // clear lines that have been filled by a locked piece
	STEP_TYPE_APPEAR, // spawn the next piece
} Step_type;

/*
 * types of changes that can happen to a playfield
 */
typedef enum {
	STEP_RESULT_MOVED,
	STEP_RESULT_NOTHING,
	STEP_RESULT_LANDED, // attempted & failed to STEP_TYPE_DOWN a piece into the ground
	STEP_RESULT_LOCKED, // the piece became part of the playfield
	STEP_RESULT_GAMEOVER,
} Step_result;

/*
 * create a playfield
 */
Field *field_create();

/*
 * destroy a playfield
 */
void field_destroy(Field *field);

/*
 * simulate one "unit" of play
 */
Step_result field_step(Field *field, Step_type type, Updates *updates);

#endif
