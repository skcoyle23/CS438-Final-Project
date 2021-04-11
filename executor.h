#ifndef BACKEND_H
#define BACKEND_H

#include "node.h"

char *searchPath(char *file);
int executeCom(int argc, char **argv);
int executeSimpleCom(struct nodeSource *node);

#endif
