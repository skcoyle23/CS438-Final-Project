#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "shell.h"
#include "symtab/symtab.h"


void init_shell() {
clear(); 
printf("****** MY SHELL *****"); 
}

int main(int argc, char **argv) {
	char *cmd;

	do {
		print_prompt1(); // Prints prompt string
		cmd = read_cmd(); // Reads the next line of input

		// Any errors of reading a command forces a shell exit
		if(!cmd) {
			exit(EXIT_SUCCESS);
		}

		// If command is empty, input is skipped and loop is 		continued
		if(cmd[0] == '\0' || strcmp(cmd, "\n")  == 0) {
			free(cmd);
			continue;
		}

		// Given an 'exit' command, shell is exited
		if(strcmp(cmd, "exit\n") == 0) {
			free(cmd);
			break;
		}

		printf("%s\n", cmd);

		free(cmd);
	} while(1);

	exit(EXIT_SUCCESS);
}

char *read_cmd(void) {
	char buf[2048]; // 2048-byte chunks
	char *ptr = NULL;
	char ptrlen = 0;

	while(fgets(buf, 1024, stdin)) {
		int buflen = strlen(buf);

		if(!ptr) {
			ptr = malloc(buflen+1); // Creates the buffer
		}
		else {
			char *ptr2 = realloc(ptr, ptrlen+buflen+1); // Extends the buffer

			if(ptr2) {
				ptr = ptr2;
			}
			else {
				free(ptr);
				ptr = NULL;
			}
		}

		if(!ptr) {
			fprintf(stderr, "error: failed to alloc buffer: %s\n", strerror(errno));
			return NULL;
		}

		strcpy(ptr+ptrlen, buf);

		if(buf[buflen-1] == '\n') {
			if(buflen == 1 || buf[buflen-2] != '\\') {
				return ptr;
			}

			ptr[ptrlen+buflen-2] = '\0';
			buflen -= 2;
			print_prompt2();
		}

		ptrlen += buflen;
	}

	return ptr;
}

initsh();
