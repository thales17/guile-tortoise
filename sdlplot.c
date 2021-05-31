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
		execlp("./sdlplotter", NULL);
		return NULL;
	}

	output = fdopen(pipes[1], "w");
	return output;
}

void draw_line (FILE* output, double x1, double y1, double x2, double y2) {
	fprintf(output, "%f %f %f %f\n",
			x1, y1, x2, y2);
	fflush(output);
}
