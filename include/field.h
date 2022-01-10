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
		STEP_TYPE_CLEAR,
		STEP_TYPE_APPEAR, // spawn the next piece
	} t;
	union {
		int movedir; // +1: right, -1: left
		int rotdir; // 0: nop (?), 1: cw, 2: 180, 3: ccw
		int appear_piece;
	};
} Step;

#define STEP_DOWN ((Step){ .t = STEP_TYPE_DOWN })
#define STEP_MOVE(m) ((Step){ .t = STEP_TYPE_MOVE, .movedir = m })
#define STEP_ROTATE(r) ((Step){ .t = STEP_TYPE_ROTATE, .rotdir = r })
#define STEP_LOCK ((Step){ .t = STEP_TYPE_LOCK })
#define STEP_CLEAR ((Step){ .t = STEP_TYPE_CLEAR })
#define STEP_APPEAR(a) ((Step){ .t = STEP_TYPE_APPEAR, .appear_piece = a })

/*
 * types of changes that can happen to a playfield
 */
typedef struct {
	enum {
		STEP_RESULT_TYPE_NOTHING,
		STEP_RESULT_TYPE_MOVED,
		STEP_RESULT_TYPE_LANDED, // attempted & failed to STEP_DOWN a piece into the ground
		STEP_RESULT_TYPE_LOCKED, // the piece became part of the playfield
		STEP_RESULT_TYPE_CLEARED,
		STEP_RESULT_TYPE_APPEARED,
		STEP_RESULT_TYPE_GAMEOVER,
	} t;
	union {
		int *board;
		int cleared;
	};
} Step_result;

#define STEP_RESULT_NOTHING ((Step_result){ .t = STEP_RESULT_TYPE_NOTHING })
#define STEP_RESULT_MOVED ((Step_result){ .t = STEP_RESULT_TYPE_MOVED })
#define STEP_RESULT_LANDED ((Step_result){ .t = STEP_RESULT_TYPE_LANDED })
#define STEP_RESULT_LOCKED ((Step_result){ .t = STEP_RESULT_TYPE_LOCKED })
#define STEP_RESULT_CLEARED(c) ((Step_result){ .t = STEP_RESULT_TYPE_CLEARED, .cleared = c })
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
 * simulate one "unit" of play
 */
Step_result field_step(Field *field, Step step);

#endif
