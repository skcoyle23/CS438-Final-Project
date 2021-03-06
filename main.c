#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "shell.h"


#define TOK_BUFSIZE 2048
#define TOK_DELIM " \t\r\n\a"

/*
 * Splits the given line into tokens
 */
char **parseLine(char *line) {
	int bufsize = TOK_BUFSIZE;
	int position = 0;
	char **tokens = malloc(bufsize * sizeof(char*));
	char *token;

	if (!tokens) {
		fprintf(stderr, "SHELL: Allocation error\n");
		exit(EXIT_FAILURE);
	}

	token = strtok(line, TOK_DELIM);
	while (token != NULL) {
		tokens[position] = token;
		position++;

		if (position >= bufsize) {
			bufsize += TOK_BUFSIZE;

			tokens = realloc(tokens, bufsize * sizeof(char*));
			if (!tokens) {
				fprintf(stderr, "SHELL: Allocation error\n");
				exit(EXIT_FAILURE);
			}
		}

		token = strtok(NULL, TOK_DELIM);
	}
	tokens[position] = NULL;
	return tokens;
}

/*
 * Function declarations for builtin shell commands
 */
int cd(char **args);

/*
 * List of builtin commands
 */
char *builtins[] = {
		"cd",
		"history"
};

int(*builtinFunctions[])(char **) = {
		&cd
		//&history
};

int numBuiltins() {
	return sizeof(builtins)/sizeof(char *);
}

/*
 * Built in change direcroty command
 * args[1] is the directory
 */
int cd(char **args) {
	if(args[1] == NULL) {
		fprintf(stderr, "Expected argument to \"cd\"\n");
	}
	else {
		if(chdir(args[1]) != 0) {
			perror("shell");
		}
	}

	return 1;
}
/*
int tab(char **args) {
	char *buffer = read_input("> ");

	if (buffer) {
		printf("You entered: %s\n", buffer);
		free(buffer);
	}
	return 0;
} */


/*
 * Launches a program and waits for it to terminate
 */
int launch(char **args) {
	pid_t pid, wpid;
	int status;

	pid = fork();

	if(pid == 0) {
		// Child process
		if(execvp(args[0], args) == -1) {
			perror("shell");
		}
		exit(EXIT_FAILURE);
	}
	else if(pid < 0) {
		// Forking error
		perror("shell");
	}
	else {
		// Parent Process
		do {
			wpid = waitpid(pid, &status, WUNTRACED);
		} while(!WIFEXITED(status) && !WIFSIGNALED(status));
	}

	return 1;
}

/*
 * Execute shell
 */
int execute(char **args) {
	int i;

	if (args[0] == NULL) {
		// An empty command was entered.
		return 1;
	}

	for (i = 0; i < numBuiltins(); i++) {
		if(strcmp(args[0], builtins[i]) == 0) {
			return (*builtinFunctions[i])(args);
		}
	}

	return launch(args);
}

void loop(void)
{
	char* input;
	char **args;
	int status;

	do {
		//print_prompt1(); // Prints the shell's prompt string; custom function
		input = readline("$ ");

		add_history(input);

		args = parseLine(input);
		status = execute(args);

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
		if(strcmp(input, "exit") == 0) {
			free(input);
			break;
		}

		free(input);
		free(args);
	} while (status);
}


int main(int argc, char **argv) {

	loop();

	return EXIT_SUCCESS;
}
