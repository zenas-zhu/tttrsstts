#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include "game.h"
#include "piece.h"

int main()
{

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "init SDL: %s\n", SDL_GetError());
		exit(1);
	}
	SDL_Window *window = SDL_CreateWindow("tttrsstts", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 160, 320, SDL_WINDOW_SHOWN);
	if (window == NULL) {
		fprintf(stderr, "create window: %s\n", SDL_GetError());
		exit(1);
	}
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL) {
		fprintf(stderr, "create renderer: %s\n", SDL_GetError());
		exit(1);
	}

	if (SDL_RenderTargetSupported(renderer) == SDL_FALSE) {
		printf("render targets are not supported: %s\nthis is a bug.\n", SDL_GetError());
		exit(1);
	}

	SDL_Rect rect = { .x = 0, .y = 0, .w = 16, .h = 16 };

	SDL_Texture *texture_ghost;
	texture_ghost = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, 16, 16);
	SDL_SetRenderTarget(renderer, texture_ghost);
	SDL_SetRenderDrawColor(renderer, 128, 128, 128, SDL_ALPHA_OPAQUE);
	SDL_RenderFillRect(renderer, &rect);

	int scancodes[8] = { SDL_SCANCODE_LEFT, SDL_SCANCODE_RIGHT, SDL_SCANCODE_DOWN, SDL_SCANCODE_UP, SDL_SCANCODE_F, SDL_SCANCODE_S, SDL_SCANCODE_D, SDL_SCANCODE_A };
	const Uint8 *keystate = SDL_GetKeyboardState(NULL);

	SDL_Texture *textures_piece[PIECES_SIZE];
	for (int i = 0; i < PIECES_SIZE; i++) {
		textures_piece[i] = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, 16, 16);
		SDL_SetRenderTarget(renderer, textures_piece[i]);
		int color = PIECE_COLORS_RGB[i];
		SDL_SetRenderDrawColor(renderer, color >> 16, color >> 8 & 0xff, color & 0xff, SDL_ALPHA_OPAQUE);
		SDL_RenderFillRect(renderer, &rect);
	}
	SDL_SetRenderTarget(renderer, NULL);

	SDL_Rect *cell_rects = malloc(sizeof(SDL_Rect) * 200); // 3.2 kb it's probably fine
	for (int i = 0; i < 200; i++) {
		int x = i % 10, y = 20 - 1 - i / 10;
		cell_rects[i] = (SDL_Rect){ .x = x * 16, .y = y * 16, .w = 16, .h = 16 };
	}

	Updates *u = updates_create();
	Inputs *p = inputs_create();
	Game *g = game_create();
	bool result = game_init(g, u);
	int *board = u->board;

	SDL_Event e;
	while (result) {
		bool stop = false;
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				stop = true;
				break;
			}
		}
		if (stop) break;
		p->keys = 0;
		for (int i = 0; i < 8; i++) {
			if (keystate[scancodes[i]]) {
				p->keys |= 1 << i;
			}
		}

		result = game_tick(g, p, u);

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(renderer);

		for (int i = 0; i < 200; i++) {
			int cell = board[i];
			if (cell == 1) {
				SDL_RenderCopy(renderer, textures_piece[u->curcolor], &rect, &cell_rects[i]);
			} else if (cell == 2) {
				SDL_RenderCopy(renderer, texture_ghost, &rect, &cell_rects[i]);
			} else if (cell) {
				SDL_RenderCopy(renderer, textures_piece[cell - 3], &rect, &cell_rects[i]);
			}
		}
		SDL_RenderPresent(renderer);
	}

	free(cell_rects);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

}
