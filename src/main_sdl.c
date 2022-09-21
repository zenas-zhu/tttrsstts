#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include "game.h"

int main()
{

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "init SDL: %s\n", SDL_GetError());
		exit(1);
	}
	SDL_Window *window = SDL_CreateWindow("tttrsstts", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 480, 360, SDL_WINDOW_SHOWN);
	if (window == NULL) {
		fprintf(stderr, "create window: %s\n", SDL_GetError());
		exit(1);
	}
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL) {
		fprintf(stderr, "create renderer: %s\n", SDL_GetError());
		exit(1);
	}

	SDL_Rect rects[8];
	for (int i = 0; i < 8; i++) {
		rects[i] = (SDL_Rect){ .x = 9 + 8 * (i % 4), .y = 9 + 8 * (i / 4), .w = 6, .h = 6 };
	}
	int scancodes[8] = { SDL_SCANCODE_A, SDL_SCANCODE_S, SDL_SCANCODE_D, SDL_SCANCODE_F, SDL_SCANCODE_LEFT, SDL_SCANCODE_DOWN, SDL_SCANCODE_UP, SDL_SCANCODE_RIGHT };
	const Uint8 *state = SDL_GetKeyboardState(NULL);

	SDL_Event e;
	int running = 1;
	while (running) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				running = 0;
			}
		}

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(renderer);

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
		for (int i = 0; i < 8; i++) {
			if (state[scancodes[i]]) {
				SDL_RenderFillRect(renderer, &rects[i]);
			} else {
				SDL_RenderDrawRect(renderer, &rects[i]);
			}
		}
		SDL_RenderPresent(renderer);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

}
