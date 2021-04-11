#ifndef SCANNER_H
#define SCANNER_H

/*
* Contains a pointer to the abstractInput in source.h
* Also contains a pointer to the token's text
*/
struct tokenInput {
    struct abstractInput *src; // Source of input
    
    char *text; // Token text
    int textLength; // Length of token text
};

// End of input token 
extern struct tokenInput endToken;

// Retrieves the next token from input 
struct tokenInput *tokenize(struct abstractInput *src);
void freeToken(struct tokenInput *tok);

#endif
