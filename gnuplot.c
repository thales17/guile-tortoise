#include <stdio.h>
#include <unistd.h>

FILE *start_plotter (const int width, const int height) {
	FILE* output;
	int pipes[2];
	pid_t pid;

	pipe(pipes);
	pid = fork();

	if (!pid) {
		dup2(pipes[0], STDIN_FILENO);
		execlp("gnuplot", NULL);
		return NULL;
	}

	output = fdopen(pipes[1], "w");

	fprintf(output, "set multiplot\n");
	fprintf(output, "set parametric\n");
	fprintf(output, "set xrange [-%d:%d]\n", width, width);
	fprintf(output, "set yrange [-%d:%d]\n", height, height);
	fprintf(output, "set size ratio -1\n");
	fprintf(output, "unset xtics\n");
	fprintf(output, "unset ytics\n");
	fflush(output);

	return output;
}

void draw_line (FILE* output, double x1, double y1, double x2, double y2) {
	fprintf(output, "plot [0:1] %f + %f * t, %f + %f * t notitle\n",
			x1, x2 - x1, y1, y2 - y1);
	fflush(output);
}
