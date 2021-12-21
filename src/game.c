#include <stdlib.h>
#include "field.h"
#include "game.h"

struct game_ {
	Field *field;
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

bool game_tick(Game *game, int key, Updates *updates)
{
	Step step;
	if (!game->started) {
		step.t = STEP_TYPE_APPEAR;
		Step_result r = field_step(game->field, step);
		updates_set_board(updates, r.board);
		updates_set_timeout(updates, 100);
		game->started = true;
		return true;
	}
	switch (key) {
		case -1: // timed out, whatvere
			// break;
		case 0:
			step.t = STEP_TYPE_DOWN;
			field_step(game->field, step);
			break;
		case 1:
			step.t = STEP_TYPE_LOCK;
			field_step(game->field, step);
			step.t = STEP_TYPE_APPEAR;
			if (field_step(game->field, step).t == STEP_RESULT_GAMEOVER) {
				return false;
			}
			break;
		case 2:
		case 3:
			step.t = STEP_TYPE_MOVE;
			step.movedir = key * 2 - 5;
			field_step(game->field, step);
			break;
		case 4:
			step.t = STEP_TYPE_ROTATE;
			step.rotdir = 1;
			field_step(game->field, step);
			break;
	}
	return true;
}
