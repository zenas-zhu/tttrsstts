#ifndef FIELD_H
#define FIELD_H

#include "updates.h"

typedef struct field_ Field;

/*
 * movements that can change a playfield, i.e. step type with associated data
 */
typedef struct {
	enum {
		STEP_TYPE_DOWN,
		STEP_TYPE_MOVE,
		STEP_TYPE_ROTATE,
		STEP_TYPE_LOCK,
		// STEP_TYPE_CLEAR, // clear lines that have been filled by a locked piece
		STEP_TYPE_APPEAR, // spawn the next piece
	} t;
	union {
		int movedir; // 1: right, -1: left
		int rotdir; // 0: nop (?), 1: cw, 2: 180, 3: ccw
		int appear_piece;
	};
} Step;

/*
 * types of changes that can happen to a playfield
 */
typedef struct {
	enum {
		STEP_RESULT_MOVED,
		STEP_RESULT_NOTHING,
		STEP_RESULT_LANDED, // attempted & failed to STEP_TYPE_DOWN a piece into the ground
		STEP_RESULT_LOCKED, // the piece became part of the playfield
		STEP_RESULT_GAMEOVER,
	} t;
	union {
		int *board;
	};
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
Step_result field_step(Field *field, Step step);

#endif
