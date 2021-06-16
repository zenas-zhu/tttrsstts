#ifndef FIELD_H
#define FIELD_H

#include <stdbool.h>
#include <ncurses.h>

typedef struct {
	WINDOW *view;
} field_t;

typedef enum {
	STEP_RESULT_DOWN,
	STEP_RESULT_LANDED,
	STEP_RESULT_GAMEOVER,
} step_result_e;

/*
 * create a playfield
 */
field_t *create_field();

/*
 * destroy a playfield
 */
void destroy_field(field_t *field);

/*
 * simulate one "unit" of play
 */
step_result_e step_field(field_t *field);

#endif
