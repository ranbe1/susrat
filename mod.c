#include <string.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

#include "debug.c"

#define NUM_MODULES 20
#define LABEL_SIZE 256
#define PATH_SIZE 500

typedef struct {
    char name[LABEL_SIZE];
    pid_t pid;
} Module;

Module *modules[NUM_MODULES];
int _modules_size = 0;

Module *
create_module() {
    Module *module = malloc(sizeof(Module));
    if (_modules_size <= NUM_MODULES) {
        modules[_modules_size] = module;
        _modules_size++;
        return module;
    }
    else {
         fprintf(stderr, "Too many modules (NUM_MODULES = %d\n)", NUM_MODULES);
         free(module);
         return NULL;
    }
}

Module *
load_module(char *mod_name, char *mod_path) {
    Module *module = create_module();

    pid_t pid = fork();
    if (pid != 0) {
        module->pid = pid;
        memset( module->name, 0, LABEL_SIZE );
        strncpy( module->name, mod_name, strlen(mod_name) );
        return module;
    }

    char fullpath[PATH_SIZE];
    void *mod;
    void* (*mod_init)();

    snprintf( fullpath, sizeof(fullpath), "%s/%s.so", mod_path, mod_name );

    mod = dlopen( fullpath, RTLD_NOW | RTLD_GLOBAL );
    if (!mod) {
         fprintf( stderr, "Can't load module \"%s\"\n", mod_name );
    }

    *(void **)(&mod_init) = dlsym( mod, "mod_init" );

    (void)(*mod_init)();

    return NULL;
}

void
clean_modules(void) {
    if (_modules_size == 0) return;

    debug("Killing all modules", DEBUG_VERBOSE);
    char message[1024];
    Module *mod = modules[_modules_size - 1];
    do {
        snprintf(message, sizeof(message), "Stopping module %s (pid: %d)", mod->name, mod->pid);
        debug(message, DEBUG_VERBOSE);
        kill(mod->pid, SIGKILL);
        free(mod);
    } while (--_modules_size);

}
