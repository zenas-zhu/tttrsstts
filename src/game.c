#include <stdio.h>
#include <stdlib.h>
#include "field.h"
#include "game.h"

#define DAS_MICROS 150000
#define ARR_MICROS 10000
#define DROP_MICROS 1000000

struct game_ {
	Field *field;
	int drop_timer;
	int das_timer;
	int last_keys;
	int das_dir;
	bool landed;
	int hold;
	bool hold_avail;
	int bag[7];
	int bag_used;
	int queue[5];
};

static bool game_das(Game *game, int keys);
static int game_cycle_piece(Game *game, Updates *updates);
static int game_gen_piece(Game *game);

Game *game_create()
{
	Game *game = malloc(sizeof(Game));
	game->field = field_create();
	game->bag_used = 7;
	for (int i = 0; i < 5; i++) {
		game->queue[i] = game_gen_piece(game);
	}
	return game;
}

void game_destroy(Game *game)
{
	field_destroy(game->field);
	free(game);
}

bool game_init(Game *game, Updates *updates)
{
	updates->queue = game->queue;
	updates->action = "";
	game->hold = -1;
	game->hold_avail = true;
	updates->hold = -1;

	Step_result r = field_step(game->field, STEP_APPEAR(game_cycle_piece(game, updates)));
	updates->board = r.board;
	game->drop_timer = DROP_MICROS;
	game->landed = false;
	updates_flag_redraw(updates);
	game->last_keys = 0;
	return true;
}

static bool checkstep(Field *field, Step step) {
	Step_result r = field_step(field, step);
	return (r.t == STEP_RESULT_TYPE_OK);
}

static char *lookup_clear_name(int cleared, bool tspin) {
	if (!tspin) {
		if (cleared > 5) cleared = 5;
		char *actiontexts[] = { "", "single", "double", "triple", "four", "undefined" };
		return actiontexts[cleared];
	} else {
		if (cleared > 4) cleared = 4;
		char *actiontexts[] = { "ts0", "tss", "tsd", "tst", "undefined" };
		return actiontexts[cleared];
	}
}

bool game_tick(Game *game, Inputs *inputs, Updates *updates)
{
	game->drop_timer -= 16667;
	int new_keys = inputs->keys & ~game->last_keys;
	// process hold if legal
	if (new_keys & 1 << GAME_KEY_HOLD && game->hold_avail) {
		game->hold_avail = false;
		int cur_active = updates->curcolor;
		int next_active = (game->hold != -1)
		                  ? game->hold
		                  : game_cycle_piece(game, updates);
		game->hold = cur_active;
		updates->hold = cur_active;
		updates->curcolor = next_active;
		updates_flag_redraw(updates);
		field_step(game->field, STEP_APPEAR(next_active));
		game->drop_timer = DROP_MICROS;
		game->landed = false;
	}
	// process move/rotate
	// if a move/rotate is performed successfully, we can stall piece landing
	bool stallable = false;
	stallable |= game_das(game, inputs->keys);
	stallable |= new_keys & (1 << GAME_KEY_CW)    && checkstep(game->field, STEP_ROTATE(1));
	stallable |= new_keys & (1 << GAME_KEY_180)   && checkstep(game->field, STEP_ROTATE(2));
	stallable |= new_keys & (1 << GAME_KEY_CCW)   && checkstep(game->field, STEP_ROTATE(3));
	// stall piece landing
	if (stallable && game->landed) {
		game->drop_timer = DROP_MICROS;
		game->landed = false;
	}
	bool timedout = (game->drop_timer <= 0);
	if (new_keys & (1 << GAME_KEY_HARD_DROP) || (timedout && game->landed)) {
		// lock piece and spawn a new one
		field_step(game->field, STEP_LOCK);
		Step_result r = field_step(game->field, STEP_CLEAR);
		updates->action = lookup_clear_name(r.cleared, r.tspin);
		r = field_step(game->field, STEP_APPEAR(game_cycle_piece(game, updates)));
		updates_flag_redraw(updates);
		if (r.t == STEP_RESULT_TYPE_GAMEOVER) {
			return false;
		}
		game->landed = false;
		game->hold_avail = true;
		game->drop_timer = DROP_MICROS;
	} else if (timedout && !game->landed) {
		// piece drops but is not locked yet
		Step_result r = field_step(game->field, STEP_DOWN);
		if (r.t == STEP_RESULT_TYPE_NOTHING) {
			game->landed = true;
		}
		game->drop_timer = DROP_MICROS;
	} else if (inputs->keys & (1 << GAME_KEY_SOFT_DROP)) {
		if (!game->landed) {
			// drop one tile now
			Step_result r = field_step(game->field, STEP_DOWN);
			if (r.t == STEP_RESULT_TYPE_OK) {
				game->drop_timer = DROP_MICROS;
			}
		}
	}
	game->last_keys = inputs->keys;
	return true;
}

static bool game_das(Game *game, int keys)
{
	int keys_down = keys & ~game->last_keys;
	int keys_up = game->last_keys & ~keys;
	int press_left = keys_down & (1 << GAME_KEY_LEFT), press_right = keys_down & (1 << GAME_KEY_RIGHT);
	int held_left = keys & (1 << GAME_KEY_LEFT), held_right = keys & (1 << GAME_KEY_RIGHT);
	int release_left = keys_up & (1 << GAME_KEY_LEFT), release_right = keys_up & (1 << GAME_KEY_RIGHT);
	bool stallable = false;
	if (press_left || press_right) {
		// start charging das
		game->das_timer = DAS_MICROS;
		if (press_right) {
			game->das_dir = +1;
			stallable = checkstep(game->field, STEP_MOVE(+1));
		} else {
			game->das_dir = -1;
			stallable = checkstep(game->field, STEP_MOVE(-1));
		}
	}
	if (held_left || held_right) {
		if (release_left) {
			game->das_timer = DAS_MICROS;
			game->das_dir = +1;
		}
		if (release_right) {
			game->das_timer = DAS_MICROS;
			game->das_dir = -1;
		}
		game->das_timer -= 16667;
		while (game->das_timer <= 0) {
			Step_result r = field_step(game->field, STEP_MOVE(game->das_dir));
			if (r.t != STEP_RESULT_TYPE_OK) {
				game->das_timer = ARR_MICROS;
				break;
			}
			stallable = true;
			game->das_timer += ARR_MICROS;
		}
	}
	return stallable;
}

static int game_cycle_piece(Game *game, Updates *updates)
{
	int next = game->queue[0];
	for (int i = 0; i < 4; i++) {
		game->queue[i] = game->queue[i+1];
	}
	game->queue[4] = game_gen_piece(game);
	updates->curcolor = next;
	return next;
}

static int game_gen_piece(Game *game)
{
	if (game->bag_used == 7) {
		for (int i = 0; i < 7; i++) {
			game->bag[i] = i;
		}
		int r = rand() % 5040;
		for (int i = 0; i < 6; i++) {
			int ri = r % (7 - i);
			r /= (7 - i);
			int t = game->bag[i];
			game->bag[i] = game->bag[ri];
			game->bag[ri] = t;
		}
		game->bag_used = 0;
	}
	return game->bag[game->bag_used++];
}
