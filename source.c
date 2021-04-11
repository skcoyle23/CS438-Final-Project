#include <errno.h>
#include "shell.h"
#include "source.h"

/*
* This class initializing the functions specified in source.h
*/

/*
* Returns the next character of input and updates the source pointer
*/
char nextChar(struct abstractInput *src) {
	// Checks for an error
    if(!src || !src-> input) {
    // Taken from the errno.h file; indicates an error occurred during a function call
        errno = ENODATA; 
        return ERRORCHAR; // Returns 0 if an error exists
    }

    char char1 = 0; // Starting position
    
    // Checking if current position equals initial source position which is -2
    if(src-> currentPos == INIT_SRC_POS) {
        src-> currentPos = -1;
    }
    else {
        char1 = src-> input[src-> currentPos];
    }
    
    // Function returns special character when we reach the last character
    if(++src-> currentPos >= src-> inSize) {
        src-> currentPos = src-> inSize;
        return LASTCHAR; // Initialized as -1 in source.h
    }

    return src-> input[src-> currentPos];
}


// Gets the character at the previous position 
void previousChar(struct abstractInput *src) {
	// Returns if the character is at position 0 = no previous character
    if(src-> currentPos < 0) {
        return;
    }

    src-> currentPos--;
}

/*
* Similar to the nextChar, but does not redirect the source pointer
* Looks at the character at current position
*/
char checkChar(struct abstractInput *src) {
// Returns 0 if an error exists
    if(!src || !src-> input) {
        errno = ENODATA;
        return ERRCHAR;
    }

    long position = src-> currentPos;

    if(position == INIT_SRC_POS) {
        position++;
    }
    position++;

    if(position >= src-> inSize) {
        return LASTCHAR;
    }

    return src->input[position];
}

/*
* Skips all whitespace characters using the checkChar function above
*/
void skipSpaces(struct abstractInput *src) {
    char c;

    if(!src || !src-> input) {
        return;
    }

    while(((c = checkChar(src)) != LASTCHAR) && (c == ' ' || c == '\t'))
    {
        next_char(src);
    }
}
