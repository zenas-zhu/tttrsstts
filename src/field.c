#include <stdlib.h>
#include <stdbool.h>
#include <ncurses.h>
#include "field.h"

struct field {
	field_t field;
	int i;
	int j;
	bool cells[40][10];
};
#define FIELD_CAST(F) ((struct field *)(F))

static void field_show(field_t *field, int y, int x, char *s);
static bool field_occupied(field_t *field, int y, int x);

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
	FIELD_CAST(field)->j = 4;
	field_show(field, 19, 4, "[]");
	wrefresh(field->view);
	return field;
}

void destroy_field(field_t *field)
{
	delwin(field->view);
	free(field);
}

step_result_e step_field(field_t *field, step_type_e type)
{
	int cur_i = FIELD_CAST(field)->i;
	int cur_j = FIELD_CAST(field)->j;
	field_show(field, cur_i, cur_j, "  ");
	int next_i = cur_i, next_j = cur_j;
	switch (type)
	{
		case STEP_TYPE_DOWN: next_i -= 1; break;
		case STEP_TYPE_LEFT: next_j -= 1; break;
		case STEP_TYPE_RIGHT: next_j += 1; break;
	}
	step_result_e result;
	if (field_occupied(field, next_i, next_j)) {
		if (type == STEP_TYPE_DOWN) {
			FIELD_CAST(field)->cells[cur_i][cur_j] = true;
			field_show(field, cur_i, cur_j, "##");
			FIELD_CAST(field)->i = 19;
			FIELD_CAST(field)->j = 4;
			if (FIELD_CAST(field)->cells[19][4]) {
				result = STEP_RESULT_GAMEOVER;
			} else {
				field_show(field, 19, 4, "[]");
				result = STEP_RESULT_LANDED;
			}
		} else {
			result = STEP_RESULT_NOTHING;
		}
	} else {
		FIELD_CAST(field)->i = next_i;
		FIELD_CAST(field)->j = next_j;
		field_show(field, next_i, next_j, "[]");
		result = STEP_RESULT_DOWN;
	}
	wrefresh(field->view);
	return result;
}

static bool field_occupied(field_t *field, int y, int x)
{
	return (y < 0) || (x < 0) || (x > 10) || FIELD_CAST(field)->cells[y][x];
}

static void field_show(field_t *field, int y, int x, char *s)
{
	mvwprintw(field->view, 20 - y, x * 2 + 2, s);
}
