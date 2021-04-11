#include <unistd.h>
#include "shell.h"
#include "parser.h"
#include "scanner.h"
#include "node.h"
#include "source.h"


struct nodeSource *parseSimpleCommand(struct tokenInput *tok) {
    if(!tok) {
        return NULL;
    }
    
    struct nodeSource *input = newNode(NODE_COMMAND);
   
    if(!input) {
        freeToken(tok);
        return NULL;
    }
    
    struct abstractInput *src = tok->src;
    
    /*
    * Calls tokenize to retrieve input tokens one-by-one until a newline token 
    * is retrieved or end of input is reached
    */
    do {
        if(tok->text[0] == '\n') {
            freeToken(tok);
            break;
        }

        struct nodeSource *word = newNode(NODE_VAR);
        
        // Creating a node tree for command execution
        if(!word) {
            freeNodeTree(input);
            freeToken(tok);
            return NULL;
        }
        
        setNodeVal(word, tok->text);
        addChildNode(input, word); // Node must contain input info and pointer

        freeToken(tok);

    } while((tok = tokenize(src)) != &endToken);

    return input;
}
