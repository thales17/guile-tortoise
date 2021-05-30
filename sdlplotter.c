#include <stdio.h>
#include <SDL.h>

#define WIDTH 640
#define HEIGHT 480

SDL_Window *window;
SDL_Renderer *renderer;

int main (int argc, char **argv) {
	SDL_Rect r = {180, 100, 75, 75};
	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("sdlplotter",
							  SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
							  WIDTH, HEIGHT,
							  SDL_WINDOW_OPENGL |
							  SDL_WINDOW_ALLOW_HIGHDPI);

	if (window == NULL) {
		printf("Could not create window: %s\n", SDL_GetError());
	}

	renderer = SDL_CreateRenderer(window, -1,
								  SDL_RENDERER_ACCELERATED |
								  SDL_RENDERER_PRESENTVSYNC);
	SDL_RenderSetLogicalSize(renderer, WIDTH, HEIGHT);

	while (1) {
		SDL_Event e;
		while (SDL_PollEvent(&e)) {}
		if (e.type == SDL_QUIT) {
			break;
		}

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

		SDL_RenderFillRect(renderer, &r);
		SDL_RenderPresent(renderer);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
