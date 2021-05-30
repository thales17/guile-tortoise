CFLAGS =
GUILE_CFLAGS = `guile-config compile`
SDL2_CFLAGS = `sdl2-config --cflags`
GUILE_LIBS = `guile-config link`
SDL2_LIBS = `sdl2-config --libs`
CC = gcc

build: tortoise

tortoise:
	@mkdir -p bin/
	@$(CC) -o bin/$@ tortoise.c gnuplot.c $(CFLAGS) $(GUILE_CFLAGS) -lm $(GUILE_LIBS)

sdlplotter:
	@mkdir -p bin/
	@$(CC) -o bin/$@ sdlplotter.c $(CFLAGS) $(SDL2_CFLAGS) $(SDL2_LIBS)
