#ifndef FIELD_H
#define FIELD_H

#include <stdbool.h>
#include <ncurses.h>

typedef struct {
	WINDOW *view;
} field_t;

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
bool step_field(field_t *field);

#endif
