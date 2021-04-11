#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "shell.h"

int main(int argc, char **argv) {
    
    char *input;
    
    do {
    	print_prompt1(); // Prints the shell's prompt string; custom function
        
        input = read_input(); // Reads next line input; custom function
      
      	// Exit if command has an error
        if(!input){
            exit(EXIT_SUCCESS);
        }
      
      	// Skip input and continue w/ loop if command is empty
        if(input[0] == '\0' || strcmp(input, "\n") == 0){
            free(input);
            continue;
        }
        
        // strcmp = compares two strings
        if(strcmp(input, "exit\n") == 0) {
            free(input);
            break;
        }
        
        printf("%s\n", input);

        free(input);
    } while(1);
    
    exit(EXIT_SUCCESS);
}

char *read_input(void){
    
    char input[2048]; // Reads in 2048-byte chunks into buffer
    char *pointer = NULL;
    char pointerLength = 0;
    
    /*
    * fgets reads a line from a specified stream and stores it into the string 
    * specified as the first argument = buf.  
    * Reads 2048 characters
    * The pointer file is stdin = standard input
    */
    while(fgets(input, 2048, stdin)){
        int inLength = strlen(input); // Gets input length
        
        /* Creating a memory pointer to allocated memory with a length one 		* greater than the input w/ malloc
        * Returns a pointer only if input exists/not NULL
        */
        if(!pointer){
            pointer = malloc(inLength+1); // malloc(size) 
        }
        else{
        /*
        * With realloc it attempts to resize the memory block pointed to by 		* pointer that was previously allocated with malloc
        */
            char *pointer2 = realloc(pointer, pointerLength + inLength+1);
           
            if(pointer2) {
                pointer = pointer2;
            }
            else{
                free(pointer);
                pointer = NULL;
            }
        }
        
        // For errors related to memory and buffers; return null
        if(!pointer){
            fprintf(stderr, "error: failed to alloc buffer: %s\n", 			strerror(errno));
            return NULL;
        }
        
        // Copies string to pointer destination and uses input as the source 	to be copied
        strcpy(pointer + pointerLength, input);
        
        /*
        * Terminates each line with a backslash character then starts a newline
        * the -2 discards the two slashes and continues to read input
        * If the input of the buffer ends w/ "\n", prints prompt 2 and continue
        * reading input. 
        */
        if(input[inLength -1] == '\n') {
            if(inLength == 1 || input[inLength - 2] != '\\') {
                return pointer; // If last \n is not escaped, return to main
            }
            
            pointer[pointerLength + inLength - 2] = '\0';
            inLength -= 2;
            
            print_prompt2();
        }
        pointerLength += inLength;
    }
    return pointer;
}
