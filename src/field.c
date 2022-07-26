#include <stdlib.h>
#include <stdbool.h>
#include <ncurses.h>
#include "field.h"
#include "piece.h"

struct field_ {
	int piece_id;
	int piece_r;
	int piece_c;
	int piece_o;
	int ghost_r;
	int cells[40][10];
	bool tspin;
};

static bool field_piece_blocked(Field *field, int p, int r, int c, int o);
static void field_minos_do(int p, int r, int c, int o, void (*cb)(int, int, void *), void *ctx);
static void field_minos_fill(Field *field, int p, int r, int c, int o, int color);

Field *field_create()
{
	Field *field = malloc(sizeof(Field));
	for (int i = 0; i < 400; i++) {
		field->cells[0][i] = 0;
	}
	field->piece_id = -1;
	return field;
}

void field_destroy(Field *field)
{
	free(field);
}

Step_result field_step(Field *field, Step step)
{
	int cur_r = field->piece_r,
	    cur_c = field->piece_c,
	    cur_o = field->piece_o;
	/* compute the "next position" of the piece should be,
	   exactly what that means depends on the step.
	   for DOWN/MOVE/ROTATE, the position of the piece after the step.
	   for LOCK, where the piece will be locked and merged into the playfield.
	   for APPEAR, where the next piece will be. */
	int next_r = cur_r, next_c = cur_c, next_o = cur_o;
	switch (step.t)
	{
		case STEP_TYPE_DOWN: next_r -= 1; break;
		case STEP_TYPE_MOVE: next_c += step.movedir; break;
		case STEP_TYPE_ROTATE: next_o = (next_o + step.rotdir) % 4; break;
		case STEP_TYPE_LOCK: next_r = field->ghost_r; break;
		case STEP_TYPE_CLEAR: break;
		case STEP_TYPE_APPEAR:
			next_r = 21 - PIECE_SIZES[step.appear_piece];
			next_c = (10 - PIECE_SIZES[step.appear_piece]) / 2;
			next_o = 0;
			break;
	}

	Step_result result;
	switch (step.t)
	{
		case STEP_TYPE_DOWN:
		case STEP_TYPE_MOVE:
		case STEP_TYPE_ROTATE:
			if (field->piece_id == -1) {
				result = STEP_RESULT_NOTHING;
				break;
			}
			if (field_piece_blocked(field, field->piece_id, next_r, next_c, next_o)) {
				// attempt rotate with kick
				bool unkickable = true;
				if (step.t == STEP_TYPE_ROTATE) {
					int kicks_id = KICKS_IDS[field->piece_id];
					int kicks_count = KICKS_SIZES[kicks_id];
					int kicks_offset = ((step.rotdir - 1) * 4 + cur_o) * kicks_count * 2;
					for (int i = 0; i < kicks_count; i++) {
						int kick_r = KICKS[kicks_id][kicks_offset + i * 2 + 1];
						int kick_c = KICKS[kicks_id][kicks_offset + i * 2 + 0];
						if (!field_piece_blocked(field, field->piece_id, cur_r + kick_r, cur_c + kick_c, next_o)) {
							next_r = cur_r + kick_r;
							next_c = cur_c + kick_c;
							unkickable = false;
							field->tspin = (field->piece_id == T_ID);
							break;
						}
					}
				}
				if (unkickable) {
					result = STEP_RESULT_NOTHING;
					break;
				}
			} else {
				field->tspin = (step.t == STEP_TYPE_ROTATE && field->piece_id == T_ID);
			}
			// clear the active piece and ghost
			field_minos_fill(field, field->piece_id, cur_r, cur_c, cur_o, 0);
			field_minos_fill(field, field->piece_id, field->ghost_r, cur_c, cur_o, 0);
			// calculate new ghost position
			if (step.t != STEP_TYPE_DOWN) {
				field->ghost_r = next_r;
				while (!field_piece_blocked(field, field->piece_id, field->ghost_r - 1, next_c, next_o)) {
					field->ghost_r -= 1;
				}
			}
			field->piece_r = next_r;
			field->piece_c = next_c;
			field->piece_o = next_o;
			// draw new active piece and ghost
			field_minos_fill(field, field->piece_id, field->ghost_r, next_c, next_o, 2);
			field_minos_fill(field, field->piece_id, next_r, next_c, next_o, 1);
			result = STEP_RESULT_OK;
			break;
		case STEP_TYPE_LOCK:
			if (field->piece_id == -1) {
				result = STEP_RESULT_NOTHING;
				break;
			}
			// draw new piece right over the ghost piece
			field_minos_fill(field, field->piece_id, cur_r, cur_c, cur_o, 0);
			field_minos_fill(field, field->piece_id, next_r, next_c, next_o, 3 + field->piece_id);
			field->piece_id = -1;
			result = STEP_RESULT_OK;
			break;
		case STEP_TYPE_CLEAR:
			if (field->piece_id != -1) {
				result = STEP_RESULT_NOTHING;
				break;
			}
			int src = 0, dst = 0, cleared = 0;
			while (dst < 40) {
				if (src == 40) {
					// fill remaining space at the top with nothing
					for (int c = 0; c < 10; c++) {
						field->cells[dst][c] = 0;
					}
					dst += 1;
				} else {
					// move down this line, or leave it to be overwritten
					bool clear = true;
					for (int c = 0; c < 10; c++) {
						if (field->cells[src][c] < 3) {
							clear = false;
							break;
						}
					}
					if (clear) {
						cleared += 1;
					} else {
						for (int c = 0; c < 10; c++) {
							field->cells[dst][c] = field->cells[src][c];
						}
						dst += 1;
					}
					src += 1;
				}
			}
			result = STEP_RESULT_CLEARED(cleared, field->tspin);
			break;
		case STEP_TYPE_APPEAR:
			if (field->piece_id != -1) {
				// clear the active piece and ghost
				field_minos_fill(field, field->piece_id, cur_r, cur_c, cur_o, 0);
				field_minos_fill(field, field->piece_id, field->ghost_r, cur_c, cur_o, 0);
			}
			field->piece_id = step.appear_piece;
			// can next piece spawn?
			bool gameover = field_piece_blocked(field, field->piece_id, next_r, next_c, next_o);
			if (gameover) {
				result = STEP_RESULT_GAMEOVER((int *)field->cells);
			} else {
				result = STEP_RESULT_APPEARED((int *)field->cells);
				// calculate and draw new ghost position
				field->ghost_r = next_r;
				while (!field_piece_blocked(field, field->piece_id, field->ghost_r - 1, next_c, next_o)) {
					field->ghost_r -= 1;
				}
				field_minos_fill(field, field->piece_id, field->ghost_r, next_c, next_o, 2);
			}
			// draw new piece
			field_minos_fill(field, field->piece_id, next_r, next_c, next_o, 1);
			field->piece_r = next_r;
			field->piece_c = next_c;
			field->piece_o = next_o;
			field->tspin = false;
			break;
	}

	return result;

}

struct checkblock_ctx { Field *f; bool blocked; };

static void checkblock(int r, int c, void *ctx)
{
	struct checkblock_ctx *x = ctx;
	bool cellblocked = (r < 0) || (r >= 40) || (c < 0) || (c >= 10) || x->f->cells[r][c] >= 3;
	x->blocked |= cellblocked;
}

static bool field_piece_blocked(Field *field, int p, int r, int c, int o)
{
	struct checkblock_ctx x = { .f = field, .blocked = false };
	field_minos_do(p, r, c, o, checkblock, &x);
	return x.blocked;
}

static void field_minos_do(int p, int r, int c, int o, void (*cb)(int, int, void *), void *ctx)
{
	for (int y = 0; y < PIECE_SIZES[p]; y++) {
		for (int x = 0; x < PIECE_SIZES[p]; x++) {
			if (PIECES[p][y * PIECE_SIZES[p] + x]) {
				int ny = PIECE_SIZES[p] - y - 1;
				int nx = PIECE_SIZES[p] - x - 1;
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

struct fillcell_ctx { Field *f; int color; };

static void fillcell(int r, int c, void *ctx)
{
	struct fillcell_ctx *x = ctx;
	x->f->cells[r][c] = x->color;
}

static void field_minos_fill(Field *field, int p, int r, int c, int o, int color)
{
	struct fillcell_ctx x = { .f = field, .color = color };
	field_minos_do(p, r, c, o, fillcell, &x);
}
