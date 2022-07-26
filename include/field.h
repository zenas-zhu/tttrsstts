#ifndef FIELD_H
#define FIELD_H

#include "updates.h"

typedef struct field_ Field;

/*
 * the smallest unit of change that can happen to a playfield.
 * composed of a step type with optional associated data.
 *
 * a field that has just been created has no active piece.
 *
 * - DOWN/MOVE/ROTATE/LOCK will only work when there is an active piece,
 *   or else the step result type will be NOTHING.
 * - CLEAR will only work when there is *not* an active piece,
 *   or else the step result type will be NOTHING.
 * - LOCK will cause the field to no longer have an active piece.
 * - APPEAR will cause the field to have an active piece,
 *   whether or not there was one before the step.
 */
typedef struct {
	enum {
		STEP_TYPE_DOWN,
		STEP_TYPE_MOVE,
		STEP_TYPE_ROTATE,
		STEP_TYPE_LOCK,
		STEP_TYPE_CLEAR,
		STEP_TYPE_APPEAR, // spawn the next piece
	} t;
	union {
		int movedir; // +1: right, -1: left
		int rotdir; // 0: nop (?), 1: cw, 2: 180, 3: ccw
		int appear_piece;
	};
} Step;

/*
 * shorthands for steps.
 */
#define STEP_DOWN ((Step){ .t = STEP_TYPE_DOWN })
#define STEP_MOVE(m) ((Step){ .t = STEP_TYPE_MOVE, .movedir = m })
#define STEP_ROTATE(r) ((Step){ .t = STEP_TYPE_ROTATE, .rotdir = r })
#define STEP_LOCK ((Step){ .t = STEP_TYPE_LOCK })
#define STEP_CLEAR ((Step){ .t = STEP_TYPE_CLEAR })
#define STEP_APPEAR(a) ((Step){ .t = STEP_TYPE_APPEAR, .appear_piece = a })

/*
 * result of a requested step.
 * composed of a step result type with optional associated data.
 *
 * - NOTHING is returned in response to MOVE/ROTATE when
 *   the movement would cause a collision.
 * - OK is returned in response to DOWN/MOVE/ROTATE/LOCK when successful.
 * - GAMEOVER occurs in response to APPEAR,
 *   when there is no space to spawn the new piece.
 */
typedef struct {
	enum {
		STEP_RESULT_TYPE_NOTHING,
		STEP_RESULT_TYPE_OK,
		STEP_RESULT_TYPE_CLEARED,
		STEP_RESULT_TYPE_APPEARED,
		STEP_RESULT_TYPE_GAMEOVER,
	} t;
	union {
		int *board;
		struct {
			int cleared;
			bool tspin;
		};
	};
} Step_result;

/*
 * shorthands for step results.
 */
#define STEP_RESULT_NOTHING ((Step_result){ .t = STEP_RESULT_TYPE_NOTHING })
#define STEP_RESULT_OK ((Step_result){ .t = STEP_RESULT_TYPE_OK })
#define STEP_RESULT_CLEARED(c, ts) ((Step_result){ .t = STEP_RESULT_TYPE_CLEARED, .cleared = c, .tspin = ts })
#define STEP_RESULT_APPEARED(b) ((Step_result){ .t = STEP_RESULT_TYPE_APPEARED, .board = b })
#define STEP_RESULT_GAMEOVER(b) ((Step_result){ .t = STEP_RESULT_TYPE_GAMEOVER, .board = b })

/*
 * create a playfield
 */
Field *field_create();

/*
 * destroy a playfield
 */
void field_destroy(Field *field);

/*
 * apply a step to the playfield
 */
Step_result field_step(Field *field, Step step);

#endif
