#ifndef NODE_H
#define NODE_H

// Deines the types of nodes
enum nodeType {
    NODE_COMMAND, // Simple command
    NODE_VAR,  // Variable name
};


// Defines the TYPES of values that can be stored
enum valueType {
    VAL_SINT = 1, // Signed integer
    VAL_UINT, // Unsigned integer
    VAL_SLLONG, // Signed long
    VAL_ULLONG,  // Unsigned long
    VAL_FLOAT, // Decimal/floating point
    VAL_LDOUBLE, // Long double
    VAL_CHR, // Character
    VAL_STR, // String
};


// Defines the value we can STORE in a given node structure; can only have one type
union symVal {
    long sint;
    unsigned int uint;
    long long sllong;
    unsigned long long ullong;
    double sfloat;
    long double ldouble;
    char chr;
    char *str;
};


struct nodeSource {
    enum nodeType type; // Type of node
    enum valueType valType; // Node value type
    union symVal val; // Value of node
    int children; // Number of children nodes
    
    struct nodeSource *firstChild; // First child node
    struct nodeSource *nextSibling, *prevSibling; // Sibling pointers
};

struct nodeSource *newNode(enum nodeType type);
void addChildNode(struct nodeSource *parent, struct nodeSource *child);
void freeNodeTree(struct nodeSource *node);
void setNodeVal(struct nodeSource *node, char *val);

/*
* For simple commands, all nodes will haev a type, valType (VAL_STR), and
*  pointer to string value (val.str)
*/
#endif
