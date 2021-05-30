#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>

#include "common/prompt.h"

#define WIDTH 640
#define HEIGHT 480

SDL_Window *window;
SDL_Renderer *renderer;

int isValid (const char *input, void *userData) {
	int doubleCount = 0;
	double result;
	char *startPtr = input;
	char *endPtr;
	while (doubleCount < 4) {
		result = strtod(startPtr, &endPtr);
		if (result == 0) {
			if (endPtr == startPtr) {
				break;
			}
		}
		((double *)userData)[doubleCount] = result;
		doubleCount++;
		startPtr = endPtr;
	}

	return doubleCount == 4;
}

static int inputThread (void *ptr) {
	char input[20];
	double line[4];
	while (1) {
		promptUser("> ", isValid, input, line);
		printf("x1: %f y1: %f x2: %f y2: %f\n",
			   line[0], line[1], line[2], line[3]);
	}
}

int main (int argc, char **argv) {
	SDL_Rect r = {180, 100, 75, 75};
	SDL_Thread *thread;
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
	if (renderer == NULL) {
		printf("Count not create renderer: %s\n", SDL_GetError());
	}
	
	SDL_RenderSetLogicalSize(renderer, WIDTH, HEIGHT);

	thread = SDL_CreateThread(inputThread, "inputThread", (void *)NULL);
	if (thread == NULL) {
		printf("Coult not create thread: %s\n", SDL_GetError());
	} else {
		SDL_DetachThread(thread);
	}
	
	while (1) {
		SDL_Event e;
		while (SDL_PollEvent(&e)) {}
		if (e.type == SDL_QUIT) {
			//TODO: cancel thread here
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
