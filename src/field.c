#include <stdlib.h>
#include <stdbool.h>
#include <ncurses.h>
#include "field.h"

struct field {
	field_t field;
	int i;
	bool cells[40][10];
};
#define FIELD_CAST(F) ((struct field *)(F))

static void field_show(field_t *field, int x, int y, char *s);

field_t *create_field()
{
	field_t *field = malloc(sizeof(struct field));
	field->view = newwin(22, 24, 0, 0);
	box(field->view, 0, 0);
	srand(0);
	for (int i = 0; i < 10; i++) {
		int height = rand() % 4;
		for (int j = 0; j < 40; j++) {
			bool occupied = j < height;
			FIELD_CAST(field)->cells[j][i] = occupied;
			if (j < 20) {
				field_show(field, j, i, occupied ? "##" : "  ");
			}
		}
	}
	FIELD_CAST(field)->i = 19;
	field_show(field, 19, 0, "[]");
	wrefresh(field->view);
	return field;
}

void destroy_field(field_t *field)
{
	delwin(field->view);
	free(field);
}

bool step_field(field_t *field)
{
	field_show(field, FIELD_CAST(field)->i, 0, "  ");
	int next_i = FIELD_CAST(field)->i - 1;
	bool landed = FIELD_CAST(field)->cells[next_i][0];
	if (!landed) {
		FIELD_CAST(field)->i = next_i;
	}
	field_show(field, FIELD_CAST(field)->i, 0, landed ? "##" : "[]");
	wrefresh(field->view);
	return !landed;
}

static void field_show(field_t *field, int x, int y, char *s)
{
	mvwprintw(field->view, 20 - x, y * 2 + 2, s);
}
