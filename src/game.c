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
		updates_set_action(updates, "");
		game->drop_timer = DROP_MILLIS;
		game->started = true;
		game->landed = false;
		return true;
	}
	game->drop_timer -= inputs_get_millis(inputs);
	bool timedout = (game->drop_timer <= 0);
	bool stallable = true;
	Action action = inputs_get_action(inputs);
	switch (action) {
		case ACTION_LEFT:  field_step(game->field, STEP_MOVE(-1));  break;
		case ACTION_RIGHT: field_step(game->field, STEP_MOVE(+1));  break;
		case ACTION_CW:    field_step(game->field, STEP_ROTATE(1)); break;
		case ACTION_180:   field_step(game->field, STEP_ROTATE(2)); break;
		case ACTION_CCW:   field_step(game->field, STEP_ROTATE(3)); break;
		default: stallable = false;
	}
	if (stallable && game->landed) { // move or rotate
		// reset landed state
		game->drop_timer = DROP_MILLIS;
		game->landed = false;
	}
	if (action == ACTION_SOFT_DROP) {
		Step_result r = field_step(game->field, STEP_DOWN);
		if (r.t == STEP_RESULT_TYPE_MOVED) {
			game->drop_timer = DROP_MILLIS;
		}
	} else if (timedout && !game->landed) {
		Step_result r = field_step(game->field, STEP_DOWN);
		if (r.t == STEP_RESULT_TYPE_LANDED) {
			game->landed = true;
		}
	} else if (action == ACTION_HARD_DROP || (timedout && game->landed)) {
		field_step(game->field, STEP_LOCK);
		Step_result r = field_step(game->field, STEP_CLEAR);
		if (r.cleared > 4) r.cleared = 4;
		char *actiontexts[] = { "", "single", "double", "triple", "four" };
		updates_set_action(updates, actiontexts[r.cleared]);
		r = field_step(game->field, STEP_APPEAR);
		if (r.t == STEP_RESULT_TYPE_GAMEOVER) {
			return false;
		}
		game->landed = false;
	}
	if (action == ACTION_HARD_DROP || timedout) {
		game->drop_timer = DROP_MILLIS;
	}
	updates_set_timeout(updates, game->drop_timer);
	return true;
}
