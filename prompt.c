#include <stdio.h>
#include "shell.h"

/*
* Prints the first prompt string
* Seen when the shell is waiting for a command to be entered
*/
void print_prompt1(void) {
    fprintf(stderr, "$ ");
}

/*
* Prints the second prompt string
* Printed by the shell for a multi-line command
*/
void print_prompt2(void) {
    fprintf(stderr, "> ");
}
