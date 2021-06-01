CFLAGS = -g
GUILE_CFLAGS = `guile-config2.2 compile`
SDL2_CFLAGS = `sdl2-config --cflags`
GUILE_LIBS = `guile-config2.2 link`
SDL2_LIBS = `sdl2-config --libs`
COMMON_SRC=$(wildcard common/*.c)
CC = cc

build: tortoise

tortoise:
	@mkdir -p bin/
	@$(CC) -o bin/$@ tortoise.c sdlplot.c $(CFLAGS) $(GUILE_CFLAGS) -lm $(GUILE_LIBS)

tortoise-gnuplot:
	@mkdir -p bin/
	@$(CC) -o bin/$@ tortoise.c gnuplot.c $(CFLAGS) $(GUILE_CFLAGS) -lm $(GUILE_LIBS)

sdlplotter:
	@mkdir -p bin/
	@$(CC) -o bin/$@ sdlplotter.c $(COMMON_SRC) $(CFLAGS) $(SDL2_CFLAGS) $(SDL2_LIBS)
