#ifndef FIELD_H
#define FIELD_H

#include <stdbool.h>
#include <ncurses.h>

typedef struct field_ Field;

/*
 * types of movements that can change a playfield
 */
typedef enum {
	STEP_TYPE_DOWN,
	STEP_TYPE_LEFT,
	STEP_TYPE_RIGHT,
	STEP_TYPE_CW,
	STEP_TYPE_CCW,
} Step_type;

/*
 * types of changes that can happen to a playfield
 */
typedef enum {
	STEP_RESULT_DOWN,
	STEP_RESULT_NOTHING,
	STEP_RESULT_LANDED,
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
Step_result field_step(Field *field, Step_type type);

#endif
