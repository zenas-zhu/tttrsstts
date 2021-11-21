#ifndef FIELD_H
#define FIELD_H

#include "updates.h"

typedef struct field_ Field;

typedef struct piece_table_ Piece_table;
typedef struct kick_table_ Kick_table;

/*
 * types of movements that can change a playfield
 */
typedef enum {
	STEP_TYPE_DOWN,
	STEP_TYPE_MOVE,
	STEP_TYPE_ROTATE,
	STEP_TYPE_LOCK,
	// STEP_TYPE_CLEAR, // clear lines that have been filled by a locked piece
	STEP_TYPE_APPEAR, // spawn the next piece
} Step_type;

/*
 * movements that can change a playfield, i.e. step type with associated data
 */
typedef struct {
	Step_type t;
	union {
		struct {
			int movedir; // 1: right, -1: left
		};
		struct {
			int rotdir; // 0: nop (?), 1: cw, 2: 180, 3: ccw
			Kick_table *kick_table;
		};
		struct {
			Piece_table *piece_table;
			int appear_piece;
		};
	};
} Step;

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
Step_result field_step(Field *field, Step step, Updates *updates);

#endif
