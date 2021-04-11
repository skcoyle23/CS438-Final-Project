#ifndef SOURCE_H
#define SOURCE_H

#define LASTCHAR	(-1)
#define ERRCHAR	(0)
#define INIT_SRC_POS	(-2)

/*
* Makes the job of the scanner easier
*/
struct abstractInput {   
    char *input; // Pointer the input string
    long inSize; // Length of user input
    long currentPos; // Current position in string - used to get next char
};

char nextChar(struct abstractInput *src); // Finds next character
void previousChar(struct abstractInput *src); // Previous character
char checkChar(struct abstractInput *src); // Looks at characters
void skipSpaces(struct abstractInput *src); // Finds white spaces in input and skips

#endif
