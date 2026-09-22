#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include "builtin.h"




/* return user entered line */
char *dsh_read(void) {
	char *lineptr = NULL;
	ssize_t s = 0;

	if (getline(&lineptr, &s, stdin) == -1) 
		if (feof(stdin))
			exit(EXIT_SUCCESS);
		else {
			perror("dsh_read()");
			exit(EXIT_FAILURE);
		}

	return lineptr;
}

/* return tokenized line */ 
#define DSH_PARSE_DELIM " \t\n"
#define DSH_PARSE_BUFSIZE 16

char **dsh_parse(char *line) {
	char **tokens	= NULL;
	char *token		= strtok(line, DSH_PARSE_DELIM);
	unsigned int bufsize = 0;
	size_t pos;

	for (pos=0; token != NULL; pos++) {

		if (pos >= bufsize) { 
			tokens = realloc( tokens, (bufsize += DSH_PARSE_BUFSIZE) );
			if (tokens == NULL) {
				perror("dsh_parse -> realloc()");
				free(line);
				exit(EXIT_FAILURE);
			}
		}

		tokens[pos]	= token;
		token		= strtok(NULL, DSH_PARSE_DELIM);
	}
	tokens[pos] = NULL;

	return tokens;
}

/* launch external Linux builtin program and return status */
int dsh_launch(char *argv[]) {
	pid_t p = fork();
	int wstatus;

	if (p == 0) {
		/* child process */
		execvp(argv[0], argv);
		perror("dsh launching: launched process shouldn't have returned");
		exit(EXIT_FAILURE);
	} else if (p < 0) {
		perror("can't fork process");
		return 1;
	} else {
		/* parent process */
		do {
			waitpid(p, &wstatus, WUNTRACED);
		} while (!WIFEXITED(wstatus) && !WIFSIGNALED(wstatus));
	}
	return 0;
}

/* decide between Linux and dsh builtin program, return status */
int dsh_exec(char *argv[]) {

	for (int i=0; i<dsh_builtin_n; i++) {
		if (strcmp(argv[0], builtin_str[i]) == 0) 
			return (*builtin_func[i])(argv);
	}

	return dsh_launch(argv);
}



int main(int argc, char *argv[]) {
	char	*line;
	char	**args;
	int		status;
	
	do {
		printf("> ");
		line	= dsh_read();
		args	= dsh_parse(line);	
		status	= dsh_exec(args);
	} while (!status);

	free(line);
	free(args);

	return 0;
}
