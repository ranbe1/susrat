#include "mod.h"

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
#ifdef _WIN32

	HINSTANCE process;
	void* (*mod_init)() = NULL;
	Module *module;
	char fullpath[PATH_SIZE];

	module = create_module();
	snprintf(fullpath, sizeof(fullpath), "%s%s.dll", mod_path, mod_name);
	process = LoadLibraryA(fullpath);

	if (process == NULL)
	{
		fprintf(stderr, "Can't load module \"%s\"\n", mod_name);
		exit(1);
	}

	module->process = process;
	memset(module->name, 0, LABEL_SIZE);
	strncpy(module->name, mod_name, strlen(mod_name));

	*(void **)(&mod_init) = GetProcAddress(process, "mod_init");
	(void)(*mod_init)();

	return NULL;

#endif

#ifdef linux
	Module *module = create_module();

	pid_t process = fork();
	if (process != 0) {
		module->process = process;
		memset(module->name, 0, LABEL_SIZE);
		strncpy(module->name, mod_name, strlen(mod_name));
		return module;
	}

	char fullpath[PATH_SIZE];
	void *mod;
	void* (*mod_init)();

	snprintf(fullpath, sizeof(fullpath), "%s/%s.so", mod_path, mod_name);

	mod = dlopen(fullpath, RTLD_NOW | RTLD_GLOBAL);
	if (!mod) {
		fprintf(stderr, "Can't load module \"%s\"\n", mod_name);
	}

	*(void **)(&mod_init) = dlsym(mod, "mod_init");

	(void)(*mod_init)();

	return NULL;
#endif

}

void
clean_modules(void) {
	if (_modules_size == 0) return;

	debug("Killing all modules", DEBUG_VERBOSE);
	char message[1024];
	Module *mod;
	do {
		mod = modules[_modules_size - 1];
		snprintf(message, sizeof(message), "Stopping module %s (process: %d)", mod->name, mod->process);
		debug(message, DEBUG_VERBOSE);
#ifdef linux
		kill(mod->process, SIGKILL);
#else
#ifdef _WIN32
		FreeLibrary(mod->process);
#endif
#endif
		free(mod);
	} while (--_modules_size);
}

