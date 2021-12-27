#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include "updates.h"
#include "inputs.h"

/*
 * the state of a game
 */
typedef struct game_ Game;

/*
 * create a game
 */
Game *game_create();

/*
 * destroy a game
 */
void game_destroy(Game *game);

/*
 * change a game state
 * returns false iff the game has ended
 */
bool game_tick(Game *game, Inputs *inputs, Updates *updates);

#endif
