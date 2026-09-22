#include <err.h>
#include <stdio.h>
#include <unistd.h>

static int dsh_cd(char *argv[]) {
	if (argv[1] == NULL) 
		warnx("Usage: cd <pathname>");
	else if (chdir(argv[1]) == -1)
		perror("cd");
	return 0;
}

static int dsh_help(char *argv[]) {
	printf("%s", 
		"dsh - dumb shell\n"
		"type <program name> <arguments> \n"
		"type exit to close\n"
	);

	return 0;
}

static int dsh_exit(char *argv[]) {
	printf("dsh closed\n");
	return 1;
}

char *builtin_str[] = {
	"help",
	"cd",
	"exit"
};


int (*builtin_func[])(char *[]) = {
	dsh_help,
	dsh_cd,
	dsh_exit
};

int dsh_builtin_n = sizeof(builtin_str) / sizeof(char*);



