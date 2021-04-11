#ifndef PARSER_H
#define PARSER_H

#include "source.h" // Calls struct abstractInput
#include "scanner.h" // Calls struct tokenIn

// Delcaring sole parser functions
struct nodeSource *parseSimpleCommand(struct tokenInput *tok);

#endif
