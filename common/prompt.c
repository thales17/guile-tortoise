#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include "prompt.h"

void promptUser (const char *msg, int (*validator)(const char *, void *),
				 char *result, void *userData) {
	char input[PROMPT_MAX], *p;
	int isValid = 0;

	do {
		isValid = 0;
		printf("%s", msg);

		fgets(input, sizeof(input), stdin);
		if ((p = strchr(input, '\n')) == NULL) {
			continue;
		}

		*p = '\0';

		isValid = validator(input, userData);
	} while (isValid == 0);

	strcpy(result, input);
}

int termSetEcho (int echo) {
	struct termios termInfo;
	int r = tcgetattr(STDIN_FILENO, &termInfo);
	if (r > 0) {
		return r;
	}

	if (echo) {
		termInfo.c_lflag |= ECHO;
	} else {
		termInfo.c_lflag &= ~ECHO;
	}

	return tcsetattr(STDIN_FILENO, TCSANOW, &termInfo);
}

void printWidth(const char *msg, int width) {
	int col = 0;
	for (int i = 0; i < DESC_MAX && msg[i] != '\0'; i++) {
		if (msg[i] == ' ') {
			int nextSpace = 1;
			while (i + nextSpace < DESC_MAX &&
				   msg[i + nextSpace] != ' ' &&
				   msg[i + nextSpace] != '\0') {
				nextSpace++;
			}

			if (col + nextSpace > width) {
				putchar('\n');
				col = 0;
			} else {
				putchar(' ');
			}
		} else {
			putchar(msg[i]);
			col++;
		}
	}

	putchar('\n');
}
