#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include "shell.h"
#include "node.h"
#include "executor.h"

/*
* Takes the name of a command then searches the directories listed
* Tries to find the command's executable file w/ $PATH variable
*/
char *searchPath(char *file) {
    char *PATH = getenv("PATH");
    char *p = PATH;
    char *p2;
    
    while(p && *p) {
        p2 = p;

        while(*p2 && *p2 != ':') {
            p2++;
        }
        
	int pathLength = p2-p;
       
        if(!pathLength) {
            pathLength = 1;
        }
        
        int  fileLength = strlen(file);
        char path[pathLength+1 +fileLength+1];
        
	strncpy(path, p, p2-p);
        path[p2-p] = '\0';
        
	if(p2[-1] != '/') {
            strcat(path, "/"); // Appends the string pointed to by src
        }

        strcat(path, file);
        
	struct stat existing; // Sees if a file exists w/ given pathname
	
	// Goes through all existing $PATH directories if not in first to find
	// necessary file
        if(stat(path, &existing) == 0) {
            if(!S_ISREG(existing.st_mode)) {
                errno = ENOENT; // No entry error
                
                p = p2;
                
                if(*p2 == ':') {
                    p++;
                }
                continue;
            }

            p = malloc(strlen(path)+1);
           
            if(!p) {
                return NULL;
            }
            
	    strcpy(p, path);
            return p;
        }
        // For file not found; fail to find in any directory = invalid command
        else {
            p = p2;
            if(*p2 == ':') {
                p++;
            }
        }
    }

    errno = ENOENT;
    return NULL;
}

/*
* Executes a command by calling execv() to replace current process image w/
* the new command executable
*/ 
int executeCom(int argc, char **argv)
{
	// If command name contains slash, treat as a pathname and immediately 
	// call execv()
    if(strchr(argv[0], '/')) {
        execv(argv[0], argv);
    }
    else {
    // Try to locate the command and return the full pathname to pass
        char *path = searchPath(argv[0]);
        
        if(!path) {
            return 0;
        }
        
        execv(path, argv);
        free(path);
    }
    return 0;
}

/*
* Frees memory used to store the arguments list of the last command executed
*/
static inline void freeArgv(int argc, char **argv) {
    if(!argc) {
        return;
    }

    while(argc--)  {
        free(argv[argc]);
    }
}

/*
* Main function of executor; converts command tree and converts to argument
*/
int executeSimpleCom(struct nodeSource *node)
{
    if(!node) {
        return 0;
    }

    struct nodeSource *child = node->firstChild;
    
    if(!child)  {
        return 0;
    }
    
    int argc = 0;
    long maxArgs = 2048; // Maximum arguments is 2048
    char *argv[maxArgs+1]; 
    char *str;
    
    while(child)  {
        str = child->val.str;
        argv[argc] = malloc(strlen(str)+1);
        
	if(!argv[argc])  {
            freeArgv(argc, argv);
            return 0;
        }
        
	strcpy(argv[argc], str);
	
        if(++argc >= maxArgs) {
            break;
        }
        child = child->nextSibling;
    }
    argv[argc] = NULL;

// pid_t is a data type - signed integer capable of representing process ID
    pid_t childID = 0;
    
    // Error checking
    if((childID = fork()) == 0) {
        executeCom(argc, argv);
        fprintf(stderr, "error: failed to execute command: %s\n", strerror(errno));
        
        // Command failed to execute
        if(errno == ENOEXEC) {
            exit(126);
        }
        // File doesn't exist
        else if(errno == ENOENT) {
            exit(127);
        }
        else {
            exit(EXIT_FAILURE);
        }
    }
    else if(childID < 0) {
        fprintf(stderr, "error: failed to fork command: %s\n", strerror(errno));
        return 0;
    }

    int status = 0;
    waitpid(childID, &status, 0); // Returns the process of the child 
    freeArgv(argc, argv);
    
    return 1;
}
