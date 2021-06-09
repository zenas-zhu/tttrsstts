#include <stdlib.h>
#include <stdbool.h>
#include <ncurses.h>
#include "field.h"

struct field_private {
	int i;
};

field_t *create_field()
{
	field_t *field = malloc(sizeof(*field) + sizeof(*field->p));
	field->p = (struct field_private *)((uint8_t *)field + sizeof(*field));
	field->view = newwin(22, 24, 0, 0);
	box(field->view, 0, 0);
	field->p->i = 0;
	return field;
}

void destroy_field(field_t *field)
{
	delwin(field->view);
	free(field);
}

bool step_field(field_t *field)
{
	mvwprintw(field->view, field->p->i + 1, 2, "  ");
	field->p->i++;
	mvwprintw(field->view, field->p->i + 1, 2, "[]");
	wrefresh(field->view);
	return field->p->i != 19;
}
