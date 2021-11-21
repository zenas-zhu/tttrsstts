#include <stdlib.h>
#include <stdbool.h>
#include <ncurses.h>
#include "field.h"

struct field_ {
	int piece_r;
	int piece_c;
	int piece_o;
	bool cells[40][10];
};

#define SPAWN_R 19
#define SPAWN_C 4
#define SPAWN_O 0

static bool field_occupied(Field *field, int r, int c);
static bool field_piece_blocked(Field *field, int r, int c, int o);

Field *field_create()
{
	Field *field = malloc(sizeof(Field));
	for (int i = 0; i < 400; i++) {
		field->cells[0][i] = false;
	}
	return field;
}

void field_destroy(Field *field)
{
	free(field);
}

Step_result field_step(Field *field, Step step, Updates *updates)
{
	int next_r = field->piece_r, next_c = field->piece_c, next_o = field->piece_o;
	switch (step.t)
	{
		case STEP_TYPE_DOWN: next_r -= 1; break;
		case STEP_TYPE_MOVE: next_c += step.movedir; break;
		case STEP_TYPE_ROTATE: next_o ^= (step.rotdir % 2); break;
		case STEP_TYPE_LOCK:
			while (!field_piece_blocked(field, next_r - 1, next_c, next_o)) {
				next_r -= 1;
			}
			break;
		case STEP_TYPE_APPEAR:
			next_r = SPAWN_R;
			next_c = SPAWN_C;
			next_o = SPAWN_O;
			break;
	}
	Step_result result;
	if (step.t == STEP_TYPE_LOCK) {
		field->cells[next_r][next_c] = true;
		field->cells[next_r + next_o][next_c + 1 - next_o] = true;
		updates_queue_draw(updates, field->piece_r, field->piece_c, 0);
		updates_queue_draw(updates, field->piece_r + field->piece_o, field->piece_c + 1 - field->piece_o, 0);
		updates_queue_draw(updates, next_r, next_c, 2);
		updates_queue_draw(updates, next_r + next_o, next_c + 1 - next_o, 2);
		result = STEP_RESULT_LOCKED;
	} else if (step.t == STEP_TYPE_APPEAR) {
		if (field_piece_blocked(field, next_r, next_c, next_o)) {
			result = STEP_RESULT_GAMEOVER;
		} else {
			field->piece_r = next_r;
			field->piece_c = next_c;
			field->piece_o = next_o;
			updates_queue_draw(updates, next_r, next_c, 1);
			updates_queue_draw(updates, next_r + next_o, next_c + 1 - next_o, 1);
			result = STEP_RESULT_MOVED;
		}
	} else if (field_piece_blocked(field, next_r, next_c, next_o)) {
		if (step.t == STEP_TYPE_DOWN) {
			result = STEP_RESULT_LANDED;
		} else {
			result = STEP_RESULT_NOTHING;
		}
	} else {
		updates_queue_draw(updates, field->piece_r, field->piece_c, 0);
		updates_queue_draw(updates, field->piece_r + field->piece_o, field->piece_c + 1 - field->piece_o, 0);
		updates_queue_draw(updates, next_r, next_c, 1);
		updates_queue_draw(updates, next_r + next_o, next_c + 1 - next_o, 1);
		field->piece_r = next_r;
		field->piece_c = next_c;
		field->piece_o = next_o;
		result = STEP_RESULT_MOVED;
	}
	return result;
}

static bool field_occupied(Field *field, int r, int c)
{
	return (r < 0) || (c < 0) || (c >= 10) || field->cells[r][c];
}

// TODO: works for dominos only
static bool field_piece_blocked(Field *field, int r, int c, int o)
{
	return field_occupied(field, r, c) || field_occupied(field, r + o, c + 1 - o);
}
