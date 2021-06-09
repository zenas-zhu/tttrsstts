#ifndef FIELD_H
#define FIELD_H

#include <stdbool.h>
#include <ncurses.h>

struct field_private;
typedef struct {
	WINDOW *view;
	struct field_private *p;
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
