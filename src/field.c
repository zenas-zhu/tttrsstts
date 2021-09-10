#include <stdlib.h>
#include <stdbool.h>
#include <ncurses.h>
#include "field.h"

struct field_ {
	WINDOW *view;
	int piece_r;
	int piece_c;
	bool cells[40][10];
};

#define SPAWN_R 19
#define SPAWN_C 4

static bool field_occupied(Field *field, int r, int c);
static void field_show(Field *field, int r, int c, char *s);

Field *field_create()
{
	Field *field = malloc(sizeof(Field));
	field->view = newwin(22, 24, 0, 0);
	box(field->view, 0, 0);
	for (int c = 0; c < 10; c++) {
		int height = rand() % 4 + 12;
		for (int r = 0; r < 40; r++) {
			bool occupied = r < height;
			field->cells[r][c] = occupied;
			if (r < 20) {
				field_show(field, r, c, occupied ? "##" : "  ");
			}
		}
	}
	return field;
}

void field_destroy(Field *field)
{
	delwin(field->view);
	free(field);
}

Step_result field_step(Field *field, Step_type type)
{
	int next_r = field->piece_r, next_c = field->piece_c;
	switch (type)
	{
		case STEP_TYPE_DOWN: next_r -= 1; break;
		case STEP_TYPE_LEFT: next_c -= 1; break;
		case STEP_TYPE_RIGHT: next_c += 1; break;
		case STEP_TYPE_LOCK:
			while (!field_occupied(field, next_r - 1, next_c)) {
				next_r -= 1;
			}
			break;
		case STEP_TYPE_APPEAR:
			next_r = SPAWN_R;
			next_c = SPAWN_C;
			break;
		default: ;
	}
	Step_result result;
	if (type == STEP_TYPE_LOCK) {
		field->cells[next_r][next_c] = true;
		field_show(field, field->piece_r, field->piece_c, "  ");
		field_show(field, next_r, next_c, "##");
		result = STEP_RESULT_LOCKED;
	} else if (type == STEP_TYPE_APPEAR) {
		if (field->cells[next_r][next_c]) {
			result = STEP_RESULT_GAMEOVER;
		} else {
			field->piece_r = next_r;
			field->piece_c = next_c;
			field_show(field, next_r, next_c, "[]");
			result = STEP_RESULT_MOVED;
		}
	} else if (field_occupied(field, next_r, next_c)) {
		if (type == STEP_TYPE_DOWN) {
			result = STEP_RESULT_LANDED;
		} else {
			result = STEP_RESULT_NOTHING;
		}
	} else {
		field_show(field, field->piece_r, field->piece_c, "  ");
		field_show(field, next_r, next_c, "[]");
		field->piece_r = next_r;
		field->piece_c = next_c;
		result = STEP_RESULT_MOVED;
	}
	wrefresh(field->view);
	return result;
}

static bool field_occupied(Field *field, int r, int c)
{
	return (r < 0) || (c < 0) || (c > 10) || field->cells[r][c];
}

static void field_show(Field *field, int r, int c, char *s)
{
	mvwprintw(field->view, 20 - r, c * 2 + 2, s);
}
