#ifndef SHELL_H
#define SHELL_H

void print_prompt1(void);
void print_prompt2(void);

char *read_input(void);

#include "source.h"
int parseAndExecute(struct abstractInput *src);

#endif
