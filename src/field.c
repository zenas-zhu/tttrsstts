#include <stdlib.h>
#include <stdbool.h>
#include <ncurses.h>
#include "field.h"

struct field_ {
	int piece_r;
	int piece_c;
	bool cells[40][10];
};

#define SPAWN_R 19
#define SPAWN_C 4

static bool field_occupied(Field *field, int r, int c);
static void field_draw(int r, int c, int l, Updates *updates);

Field *field_create(Updates *updates)
{
	Field *field = malloc(sizeof(Field));
	for (int c = 0; c < 10; c++) {
		int height = rand() % 4 + 12;
		for (int r = 0; r < 40; r++) {
			bool occupied = r < height;
			field->cells[r][c] = occupied;
			if (r < 20) {
				field_draw(r, c, occupied ? 2 : 0, updates);
			}
		}
	}
	return field;
}

void field_destroy(Field *field)
{
	free(field);
}

Step_result field_step(Field *field, Step_type type, Updates *updates)
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
		field_draw(field->piece_r, field->piece_c, 0, updates);
		field_draw(next_r, next_c, 2, updates);
		result = STEP_RESULT_LOCKED;
	} else if (type == STEP_TYPE_APPEAR) {
		if (field->cells[next_r][next_c]) {
			result = STEP_RESULT_GAMEOVER;
		} else {
			field->piece_r = next_r;
			field->piece_c = next_c;
			field_draw(next_r, next_c, 1, updates);
			result = STEP_RESULT_MOVED;
		}
	} else if (field_occupied(field, next_r, next_c)) {
		if (type == STEP_TYPE_DOWN) {
			result = STEP_RESULT_LANDED;
		} else {
			result = STEP_RESULT_NOTHING;
		}
	} else {
		field_draw(field->piece_r, field->piece_c, 0, updates);
		field_draw(next_r, next_c, 1, updates);
		field->piece_r = next_r;
		field->piece_c = next_c;
		result = STEP_RESULT_MOVED;
	}
	return result;
}

static bool field_occupied(Field *field, int r, int c)
{
	return (r < 0) || (c < 0) || (c > 10) || field->cells[r][c];
}

static void field_draw(int r, int c, int l, Updates *updates)
{
	Update u;
	u.update_type = UPDATE_DRAW_CELL;
	u.draw_cell_r = r;
	u.draw_cell_c = c;
	u.draw_cell_color = l;
	updates_add(updates, u);
	/* updates_add(updates, (Update){
		.update_type = UPDATE_DRAW_CELL,
		.draw_cell_r = r,
		.draw_cell_c = c,
		.draw_cell_color = l}); */
}
