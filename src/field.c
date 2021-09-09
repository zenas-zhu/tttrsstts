#include <stdlib.h>
#include <stdbool.h>
#include <ncurses.h>
#include "field.h"

struct field_ {
	WINDOW *view;
	int i;
	int j;
	bool cells[40][10];
};

static void field_show(Field *field, int y, int x, char *s);
static bool field_occupied(Field *field, int y, int x);

Field *field_create()
{
	Field *field = malloc(sizeof(Field));
	field->view = newwin(22, 24, 0, 0);
	box(field->view, 0, 0);
	srand(0);
	for (int i = 0; i < 10; i++) {
		int height = rand() % 4 + 12;
		for (int j = 0; j < 40; j++) {
			bool occupied = j < height;
			field->cells[j][i] = occupied;
			if (j < 20) {
				field_show(field, j, i, occupied ? "##" : "  ");
			}
		}
	}
	field->i = 19;
	field->j = 4;
	field_show(field, 19, 4, "[]");
	wrefresh(field->view);
	return field;
}

void field_destroy(Field *field)
{
	delwin(field->view);
	free(field);
}

Step_result field_step(Field *field, Step_type type)
{
	int next_i = field->i, next_j = field->j;
	switch (type)
	{
		case STEP_TYPE_DOWN: next_i -= 1; break;
		case STEP_TYPE_LEFT: next_j -= 1; break;
		case STEP_TYPE_RIGHT: next_j += 1; break;
		case STEP_TYPE_LOCK:
			while (!field_occupied(field, next_i - 1, next_j)) {
				next_i -= 1;
			}
			break;
		case STEP_TYPE_APPEAR:
			next_i = 19;
			next_j = 4;
			break;
		default: ;
	}
	Step_result result;
	if (type == STEP_TYPE_LOCK) {
		field->cells[next_i][next_j] = true;
		field_show(field, field->i, field->j, "  ");
		field_show(field, next_i, next_j, "##");
		result = STEP_RESULT_LOCKED;
	} else if (type == STEP_TYPE_APPEAR) {
		if (field->cells[19][4]) {
			result = STEP_RESULT_GAMEOVER;
		} else {
			field->i = 19;
			field->j = 4;
			field_show(field, 19, 4, "[]");
			result = STEP_RESULT_MOVED;
		}
	} else if (field_occupied(field, next_i, next_j)) {
		if (type == STEP_TYPE_DOWN) {
			result = STEP_RESULT_LANDED;
		} else {
			result = STEP_RESULT_NOTHING;
		}
	} else {
		field_show(field, field->i, field->j, "  ");
		field_show(field, next_i, next_j, "[]");
		field->i = next_i;
		field->j = next_j;
		result = STEP_RESULT_MOVED;
	}
	wrefresh(field->view);
	return result;
}

static bool field_occupied(Field *field, int y, int x)
{
	return (y < 0) || (x < 0) || (x > 10) || field->cells[y][x];
}

static void field_show(Field *field, int y, int x, char *s)
{
	mvwprintw(field->view, 20 - y, x * 2 + 2, s);
}
