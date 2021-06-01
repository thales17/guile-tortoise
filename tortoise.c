/* Simple backend for a Logo like tortoise drawer. */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <libguile.h>

#include "plot.h"

static const int WIDTH = 10;
static const int HEIGHT = 10;

static FILE *global_output;

static double x, y;
static double direction;
static int pendown;

static void *register_functions (void *);
static SCM tortoise_reset ();
static SCM tortoise_center ();
static SCM tortoise_pendown ();
static SCM tortoise_penup ();
static SCM tortoise_turn (SCM);
static SCM tortoise_move (SCM);

int main (int argc, char **argv) {
	global_output = start_plotter(WIDTH, HEIGHT);
	tortoise_reset();

	scm_with_guile(&register_functions, NULL);
	scm_shell(argc, argv);
	
	return EXIT_SUCCESS;
}

static void *register_functions (void *data) {
	scm_c_define_gsubr("tortoise-reset", 0, 0, 0, &tortoise_reset);
	scm_c_define_gsubr("tortoise-center", 0, 0, 0, &tortoise_center);
	scm_c_define_gsubr("tortoise-penup", 0, 0, 0, &tortoise_penup);
	scm_c_define_gsubr("tortoise-pendown", 0, 0, 0, &tortoise_pendown);
	scm_c_define_gsubr("tortoise-turn", 1, 0, 0, &tortoise_turn);
	scm_c_define_gsubr("tortoise-move", 1, 0, 0, &tortoise_move);	
	return NULL;
}

static SCM tortoise_reset () {
	fprintf(global_output, "clear\n");
	fflush(global_output);

	return tortoise_center();
}

static SCM tortoise_center () {
	x = y = 0.0;
	direction = 0.0;
	pendown = 1;

	return SCM_UNSPECIFIED;
}


static SCM tortoise_pendown () {
	SCM result = scm_from_bool(pendown);

	pendown = 1;
	
	return result;
}

static SCM tortoise_penup () {
	SCM result = scm_from_bool(pendown);
	
	pendown = 0;
	
	return result;
}

static SCM tortoise_turn (SCM degrees) {
	const double value = scm_to_double(degrees);
	direction += (M_PI / 180.0) * value;
	return scm_from_double(direction * (180.0 / M_PI));
}

static SCM tortoise_move (SCM length) {
	const double value = scm_to_double(length);
	
	double newX, newY;

	newX = x + value * cos(direction);
	newY = y + value * sin(direction);

	if (pendown) {
		draw_line(global_output, x, y, newX, newY);
	}

	x = newX;
	y = newY;

	return scm_list_2(scm_from_double(x), scm_from_double(y));
}
