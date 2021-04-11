#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include "shell.h"
#include "node.h"
#include "parser.h"

// Creates a new node and sets its type field
struct nodeSource *newNode(enum nodeType type) {
    struct nodeSource *node = malloc(sizeof(struct nodeSource));

    if(!node) {
        return NULL;
    }
    
    memset(node, 0, sizeof(struct nodeSource));
    node->type = type;
    
    return node;
}


/*
* Adds a new child node and increments the root node's children field
*/
void addChildNode(struct nodeSource *parent, struct nodeSource *child) {
    // Returns if parent or child don't exist
    if(!parent || !child) {
        return;
    }

// If root node has no childre, new child is assigned to firstChild field
// Otherwise child is appended to end of the children's list
    if(!parent->firstChild) {
        parent->firstChild = child;
    }
    else {
        struct nodeSource *sibling = parent->firstChild;
    
    	while(sibling->nextSibling) {
            sibling = sibling->nextSibling;
        }
    
    	sibling->nextSibling = child;
        child->prevSibling = sibling;
    }
    
    parent->children++;
}

/*
* Sets a node's value to the given string
*/
void setNodeVal(struct nodeSource *node, char *val) {
    node->valType = VAL_STR;

    if(!val) {
        node->val.str = NULL;
    }
    else {
    // Copies string to newly allocated memory space
        char *val2 = malloc(strlen(val)+1); 
    
    	if(!val2) {
            node->val.str = NULL;
        }
        else {
            strcpy(val2, val);
            node->val.str = val2;
        }
    }
}


/*
* Frees the memory used by a node structure
*/
void freeNodeTree(struct nodeSource *node) {
    if(!node) {
        return;
    }

    struct nodeSource *child = node->firstChild;
    
    // If node has children, function is called to free each of them; recursive
    while(child) {
        struct nodeSource *next = child->nextSibling;
        freeNodeTree(child);
        child = next;
    }
    
    if(node->valType == VAL_STR) {
        if(node->val.str) {
            free(node->val.str);
        }
    }
    free(node);
}

