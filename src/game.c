#include <stdio.h>
#include <stdlib.h>
#include "field.h"
#include "game.h"

#define DROP_MILLIS 1000

struct game_ {
	Field *field;
	int drop_timer;
	bool started;
	bool landed;
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
	if (!game->started) {
		Step_result r = field_step(game->field, STEP_APPEAR);
		updates_set_board(updates, r.board);
		updates_set_timeout(updates, DROP_MILLIS);
		game->drop_timer = DROP_MILLIS;
		game->started = true;
		game->landed = false;
		return true;
	}
	game->drop_timer -= inputs_get_millis(inputs);
	bool timedout = (game->drop_timer <= 0);
	int d = 0;
	switch (inputs_get_action(inputs)) {
		case ACTION_LEFT:  field_step(game->field, STEP_MOVE(-1));  break;
		case ACTION_RIGHT: field_step(game->field, STEP_MOVE(+1));  break;
		case ACTION_CW:    field_step(game->field, STEP_ROTATE(1)); break;
		case ACTION_180:   field_step(game->field, STEP_ROTATE(2)); break;
		case ACTION_CCW:   field_step(game->field, STEP_ROTATE(3)); break;
		case ACTION_NONE:      d = -1; break;
		case ACTION_SOFT_DROP: d = 1; break;
		case ACTION_HARD_DROP: d = 2; break;
	}
	if (!d && game->landed) { // move or rotate
		// reset landed state
		game->drop_timer = DROP_MILLIS;
		game->landed = false;
	}
	if (d == 1) {
		Step_result r = field_step(game->field, STEP_DOWN);
		if (r.t == STEP_RESULT_MOVED) {
			game->drop_timer = DROP_MILLIS;
		}
	} else if (timedout && !game->landed) {
		Step_result r = field_step(game->field, STEP_DOWN);
		if (r.t == STEP_RESULT_LANDED) {
			game->landed = true;
		}
	} else if (d == 2 || (timedout && game->landed)) {
		field_step(game->field, STEP_LOCK);
		Step_result r = field_step(game->field, STEP_APPEAR);
		if (r.t == STEP_RESULT_GAMEOVER) {
			return false;
		}
		game->landed = false;
	}
	if (d == 2 || timedout) {
		game->drop_timer = DROP_MILLIS;
	}
	updates_set_timeout(updates, game->drop_timer);
	return true;
}
