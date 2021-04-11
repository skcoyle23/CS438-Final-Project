#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "shell.h"
#include "scanner.h"
#include "source.h"

char *tokenIn = NULL; // Pointer to buffer; stores current token
int tokenInSize = 0; // Number of bytes allocated to the buffer
int tokenInIndex = -1; // Current buffer index; tells where to add next input char

// Initializing special token to signal end of input
struct tokenInput endToken = {
    .textLength = 0,
};

/*
* Adds a single character to the token buffer
*/
void addBuf(char c) {
    tokenIn[tokenInIndex++] = c;

// If buffer is full, function extends size
    if(tokenInIndex >= tokenInSize) {
        char *newSize = realloc(tokenIn, tokenInSize*2);

	// Error checking
        if(!newSize) {
            errno = ENOMEM; // Out of memory error
            return;
        }

        tokenIn = newSize;
        tokenInSize *= 2;
    }
}

/*
* Takes a string and converts it to a struct tokenInput structure
*/
struct tokenInput *createToken(char *str) {
    struct tokenInput *tok = malloc(sizeof(struct tokenInput)); 
    
    if(!tok) {
        return NULL;
    }

// Copies the character to the first n characters of the string pointed to
    memset(tok, 0, sizeof(struct tokenInput));
    
    // Sets token to length of token text
    tok-> textLength = strlen(str);
    
    char *newStr = malloc(tok->textLength+1); // New memory allocation
    
    if(!newStr) {
        free(tok);
        return NULL;
    }
    
    strcpy(newStr, str); // Copies new string and original string
    tok->text = newStr; // Sets text of token equal to new string
    
    return tok;
}


/*
* Frees memory used by token structure
*/
void freeToken(struct tokenInput *tok) {
    if(tok->text) {
        free(tok->text);
    }
    
    free(tok);
}


struct tokenInput *tokenize(struct abstractInput *src) {
    int endLoop = 0;

	// Empty data error, returns end token
    if(!src || !src->input || !src->inSize) {
        errno = ENODATA;
        return &endToken;
    }
    
    if(!tokenIn)  {
        tokenInSize = 2048;
        tokenIn = malloc(tokenInSize); // Allocates memory for token buffer
        
        // No memory error, returns end token
        if(!tokenIn)  {
            errno = ENOMEM;
            return &endToken;
        }
    }

    tokenInIndex = 0;
    tokenIn[0] = '\0';

    char next = nextChar(src);

	// Checking if there are still characters to be read, returns end token if not 
    if(next == ERRCHAR || next == LASTCHAR) {
        return &endToken;
    }

// Reads input characters one at a time
/*
* Looks for whitespace characters 
* If buffer is not empty, delimit the current token and break out of loop
* Default skips the whitespace characters and move to beginning of next token
*/
    do {
        switch(next) {
            case ' ':
            case '\t':
                if(tokenInIndex > 0) {
                    endLoop = 1;
                }
                break;
                
            case '\n':
                if(tokenInIndex > 0) {
                    previousChar(src);
                }
                else {
                    addBuf(next);
                }
                
                endLoop = 1;
                break;
                
            default:
                addBuf(next);
                break;
        }

        if(endLoop) {
            break;
        }

    } while((next = nextChar(src)) != LASTCHAR);

	// When index is 0, return end
    if(tokenInIndex == 0) {
        return &endToken;
    }
    
    if(tokenInIndex >= tokenInSize) {
        tokenInIndex--;
    }
    tokenIn[tokenInIndex] = '\0';

	// Called after we have our token
    struct tokenInput *tok = createToken(tokenIn);
    
    
    // Prints error statement
    if(!tok)  {
        fprintf(stderr, "error: failed to alloc buffer: %s\n", strerror(errno));
        return &endToken;
    }

    tok->src = src;
    return tok;
}
