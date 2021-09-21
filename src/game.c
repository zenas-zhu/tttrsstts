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
	if (!game->started) {
		field_step(game->field, STEP_TYPE_APPEAR, updates);
		updates_set_timeout(updates, 100);
		game->started = true;
		return true;
	}
	switch (key) {
		case -1: // timed out, whatvere
			break;
		case 0:
			field_step(game->field, STEP_TYPE_DOWN, updates);
			break;
		case 1:
			field_step(game->field, STEP_TYPE_LOCK, updates);
			if (field_step(game->field, STEP_TYPE_APPEAR, updates) == STEP_RESULT_GAMEOVER) {
				return false;
			}
			break;
		case 2:
			field_step(game->field, STEP_TYPE_LEFT, updates);
			break;
		case 3:
			field_step(game->field, STEP_TYPE_RIGHT, updates);
			break;
	}
	return true;
}
