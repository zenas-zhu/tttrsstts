#include <stdlib.h>
#include <stdbool.h>
#include <ncurses.h>
#include "field.h"

struct field_ {
	int piece_r;
	int piece_c;
	int piece_o;
	int cells[40][10];
};

#define SPAWN_R 18
#define SPAWN_C 4
#define SPAWN_O 0

#define PIECE_T_SIZE 3
static const bool PIECE_T[PIECE_T_SIZE][PIECE_T_SIZE] = {
	{ false, true, false },
	{ true, true, true },
	{ false, false, false },
};

static bool field_occupied(Field *field, int r, int c);
static bool field_piece_blocked(Field *field, int r, int c, int o);
static void field_minos_do(int r, int c, int o, void (*cb)(int, int, void *), void *ctx);

static void docellclear(int r, int c, void *ctx)
{ ((Field *)ctx)->cells[r][c] = 0; }
static void docellfill(int r, int c, void *ctx)
{ ((Field *)ctx)->cells[r][c] = 1; }
static void docellhi(int r, int c, void *ctx)
{ ((Field *)ctx)->cells[r][c] = 2; }

Field *field_create()
{
	Field *field = malloc(sizeof(Field));
	for (int i = 0; i < 400; i++) {
		field->cells[0][i] = 0;
	}
	return field;
}

void field_destroy(Field *field)
{
	free(field);
}

Step_result field_step(Field *field, Step step)
{
	int next_r = field->piece_r, next_c = field->piece_c, next_o = field->piece_o;
	switch (step.t)
	{
		case STEP_TYPE_DOWN: next_r -= 1; break;
		case STEP_TYPE_MOVE: next_c += step.movedir; break;
		case STEP_TYPE_ROTATE: next_o = (next_o + step.rotdir) % 4; break;
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
	bool blocked = field_piece_blocked(field, next_r, next_c, next_o);

	if (step.t == STEP_TYPE_LOCK) {
		field_minos_do(field->piece_r, field->piece_c, field->piece_o, docellclear, field);
		field_minos_do(next_r, next_c, next_o, docellfill, field);
		result = STEP_RESULT_LOCKED;
	} else if (step.t == STEP_TYPE_APPEAR) {
		if (field_piece_blocked(field, next_r, next_c, next_o)) {
			result = STEP_RESULT_GAMEOVER((int *)field->cells);
		} else {
			field_minos_do(next_r, next_c, next_o, docellhi, field);
			result = STEP_RESULT_APPEARED((int *)field->cells);
		}
	} else if (field_piece_blocked(field, next_r, next_c, next_o)) {
		if (step.t == STEP_TYPE_DOWN) {
			result = STEP_RESULT_LANDED;
		} else {
			result = STEP_RESULT_NOTHING;
		}
	} else {
		field_minos_do(field->piece_r, field->piece_c, field->piece_o, docellclear, field);
		field_minos_do(next_r, next_c, next_o, docellhi, field);
		result = STEP_RESULT_MOVED;
	}

	if (!blocked) {
		field->piece_r = next_r;
		field->piece_c = next_c;
		field->piece_o = next_o;
	}
	return result;
}

static bool field_occupied(Field *field, int r, int c)
{
	return (r < 0) || (c < 0) || (c >= 10) || field->cells[r][c] == 1;
}

struct checkblock_ctx { Field *f; bool blocked; };

static void checkblock(int r, int c, void *ctx)
{
	struct checkblock_ctx *x = ctx;
	x->blocked |= field_occupied(x->f, r, c);
}

static bool field_piece_blocked(Field *field, int r, int c, int o)
{
	struct checkblock_ctx x = { .f = field, .blocked = false };
	field_minos_do(r, c, o, checkblock, &x);
	return x.blocked;
}

static void field_minos_do(int r, int c, int o, void (*cb)(int, int, void *), void *ctx)
{
	for (int y = 0; y < PIECE_T_SIZE; y++) {
		for (int x = 0; x < PIECE_T_SIZE; x++) {
			if (PIECE_T[y][x]) {
				int ny = PIECE_T_SIZE - y - 1;
				int nx = PIECE_T_SIZE - x - 1;
				switch (o) {
					case 0: cb(r + ny, c + x, ctx); break;
					case 1: cb(r + nx, c + ny, ctx); break;
					case 2: cb(r + y, c + nx, ctx); break;
					case 3: cb(r + x, c + y, ctx); break;
				}
			}
		}
	}
}
