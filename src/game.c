#include <stdio.h>
#include <stdlib.h>
#include "field.h"
#include "game.h"

struct game_ {
	Field *field;
	int drop_timer;
	bool started;
};

Game *game_create()
{
	Game *game = malloc(sizeof(Game));
	game->field = field_create();
	game->started = false;
	return game;
}

void game_destroy(Game *game)
{
	field_destroy(game->field);
	free(game);
}

bool game_tick(Game *game, Inputs *inputs, Updates *updates)
{
	Step step;
	if (!game->started) {
		step.t = STEP_TYPE_APPEAR;
		Step_result r = field_step(game->field, step);
		updates_set_board(updates, r.board);
		updates_set_timeout(updates, 400);
		game->drop_timer = 400;
		game->started = true;
		return true;
	}
	game->drop_timer -= inputs_get_millis(inputs);
	int m = 0, r = 0, d = 0;
	switch (inputs_get_action(inputs)) {
		case ACTION_LEFT: m = -1; break;
		case ACTION_RIGHT: m = +1; break;
		case ACTION_CW: r = 1; break;
		case ACTION_180: r = 2; break;
		case ACTION_CCW: r = 3; break;
		case ACTION_SOFT_DROP: d = 1; break;
		case ACTION_HARD_DROP: d = 2; break;
		default: ;
	}
	if (m) {
		step.t = STEP_TYPE_MOVE;
		step.movedir = m;
		field_step(game->field, step);
	} else if (r) {
		step.t = STEP_TYPE_ROTATE;
		step.rotdir = r;
		field_step(game->field, step);
	} else if (d) {
		game->drop_timer = 400;
		if (d == 1) {
			step.t = STEP_TYPE_DOWN;
			field_step(game->field, step);
		} else {
			step.t = STEP_TYPE_LOCK;
			field_step(game->field, step);
			step.t = STEP_TYPE_APPEAR;
			Step_result r = field_step(game->field, step);
			if (r.t == STEP_RESULT_GAMEOVER) {
				return false;
			}
		}
	}
	if (game->drop_timer <= 0) {
		step.t = STEP_TYPE_DOWN;
		field_step(game->field, step);
		game->drop_timer = 400;
	}
	return true;
}
