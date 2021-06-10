#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL.h>

#include "common/prompt.h"

#define WIDTH 500
#define HEIGHT 500

#define CLEAR_EVENT_CODE 1

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture *texture;

Uint32 myEventType;
char userInput[500];

int isValid (const char *input, void *userData) {
	int doubleCount = 0;
	double result;
	char *startPtr = input;
	char *endPtr;

	if (strcmp(input, "clear") == 0) {
		return 1;
	}

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
	while (1) {
		double *line = calloc(4, sizeof(double));
		promptUser("", isValid, userInput, line);
		if (strcmp(userInput, "clear") == 0) {
			SDL_Event event;
			SDL_memset(&event, 0, sizeof(event));
			event.type = myEventType;
			event.user.code = CLEAR_EVENT_CODE;
			SDL_PushEvent(&event);
			continue;
		}

		SDL_Event event;
		SDL_memset(&event, 0, sizeof(event));
		event.type = myEventType;
		event.user.code = 0;
		event.user.data1 = line;
		SDL_PushEvent(&event);
	}
}

int main (int argc, char **argv) {
	double scale = 10.0;
	int centerX = (WIDTH / 2);
	int centerY = (HEIGHT / 2);
	int yPixelsPerUnit = (int)((HEIGHT / 2.0) / scale);
	int xPixelsPerUnit = (int)((WIDTH / 2.0) / scale);
	SDL_Thread *thread;
	myEventType = SDL_RegisterEvents(1);
	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("sdlplotter",
							  SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
							  WIDTH, HEIGHT,
							  SDL_WINDOW_OPENGL |
							  SDL_WINDOW_ALLOW_HIGHDPI);

	if (window == NULL) {
		printf("Could not create window: %s\n", SDL_GetError());
		return 1;
	}

	renderer = SDL_CreateRenderer(window, -1,
								  SDL_RENDERER_ACCELERATED |
								  SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL) {
		printf("Could not create renderer: %s\n", SDL_GetError());
		return 1;
	}

	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
								SDL_TEXTUREACCESS_TARGET, WIDTH, HEIGHT);
	if (texture == NULL) {
		printf("Could not create texture: %s\n", SDL_GetError());
		return 1;
	}

	SDL_SetRenderTarget(renderer, texture);
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderClear(renderer);
	SDL_SetRenderTarget(renderer, NULL);

	thread = SDL_CreateThread(inputThread, "inputThread", (void *)NULL);
	if (thread == NULL) {
		printf("Could not create thread: %s\n", SDL_GetError());
		return 1;
	} else {
		SDL_DetachThread(thread);
	}

	int count = 0;
	while (1) {
		SDL_Event e;
		int quit = 0;
		while (SDL_PollEvent(&e) == 1) {
			if (e.type == SDL_QUIT) {
				quit = 1;
				break;
			}
			if (e.type == myEventType) {
				if (e.user.code == CLEAR_EVENT_CODE) {
					SDL_SetRenderTarget(renderer, texture);
					SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
					SDL_RenderClear(renderer);
					SDL_SetRenderTarget(renderer, NULL);
				} else {
					double *lineData = (double *)e.user.data1;
					int x1 = centerX + (int)(lineData[0] * xPixelsPerUnit);
					int y1 = centerY + (int)(-1 * lineData[1] * yPixelsPerUnit);
					int x2 = centerX + (int)(lineData[2] * xPixelsPerUnit);
					int y2 = centerY + (int)(-1 * lineData[3] * yPixelsPerUnit);
					SDL_SetRenderTarget(renderer, texture);
					SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);
					SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
					SDL_SetRenderTarget(renderer, NULL);
					free(lineData);
				}
			}
		}

		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);
		if (quit > 0) {
			break;
		}
	}
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
