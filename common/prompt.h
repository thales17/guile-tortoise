#ifndef _PROMPT_H_
#define _PROMPT_H_

#include <stdint.h>

#include "constants.h"

void promptUser (const char *, int (*)(const char *, void *), char *, void *);
int termSetEcho (int echo);
void printWidth(const char *, int);

#endif
