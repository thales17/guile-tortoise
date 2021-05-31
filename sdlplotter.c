#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL.h>

#include "common/prompt.h"

#define WIDTH 640
#define HEIGHT 480

#define LINE_EVENT_CODE 1
#define CLEAR_EVENT_CODE 2

SDL_Window *window;
SDL_Renderer *renderer;
double lineData[4] = { };
Uint32 myEventType;

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
	char input[20];
	double line[4];
	while (1) {
		promptUser("", isValid, input, line);
		if (strcmp(input, "clear") == 0) {
			SDL_Event event;
			SDL_memset(&event, 0, sizeof(event));
			event.type = myEventType;
			event.user.code = CLEAR_EVENT_CODE;
			SDL_PushEvent(&event);
			continue;
		}
		/*printf("x1: %f y1: %f x2: %f y2: %f\n",
		  line[0], line[1], line[2], line[3]);*/
		lineData[0] = line[0];
		lineData[1] = line[1];
		lineData[2] = line[2];
		lineData[3] = line[3];

		SDL_Event event;
		SDL_memset(&event, 0, sizeof(event));
		event.type = myEventType;
		event.user.code = LINE_EVENT_CODE;

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
	}

	renderer = SDL_CreateRenderer(window, -1,
								  SDL_RENDERER_ACCELERATED |
								  SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL) {
		printf("Count not create renderer: %s\n", SDL_GetError());
	}

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);

	SDL_RenderSetLogicalSize(renderer, WIDTH, HEIGHT);

	thread = SDL_CreateThread(inputThread, "inputThread", (void *)NULL);
	if (thread == NULL) {
		printf("Coult not create thread: %s\n", SDL_GetError());
	} else {
		SDL_DetachThread(thread);
	}

	while (1) {
		SDL_Event e;
		int quit = 0;
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				quit = 1;
				break;
			}

			if (e.type == myEventType) {
				if (e.user.code == CLEAR_EVENT_CODE) {
					SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
					SDL_RenderClear(renderer);
					SDL_RenderPresent(renderer);
				} else if (e.user.code == LINE_EVENT_CODE) {
					SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
					int x1 = centerX + (int)(lineData[0] * xPixelsPerUnit);
					int y1 = centerY + (int)(lineData[1] * yPixelsPerUnit);
					int x2 = centerX + (int)(lineData[2] * xPixelsPerUnit);
					int y2 = centerY + (int)(lineData[3] * yPixelsPerUnit);
					SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
					SDL_RenderPresent(renderer);
				}
			}
		}
		if (quit > 0) {
			break;
		}

		SDL_Delay(50);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
