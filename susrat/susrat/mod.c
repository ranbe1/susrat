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

	HINSTANCE hDLL;
	void* (*mod_init)() = NULL;
	Module *module;
	char fullpath[PATH_SIZE];

	module = create_module();
	snprintf(fullpath, sizeof(fullpath), "%s%s.dll", mod_path, mod_name);
	hDLL = LoadLibraryA(fullpath);

	if (hDLL == NULL)
	{
		fprintf(stderr, "Can't load module \"%s\"\n", mod_name);
		exit(1);
	}

	module->hDLL = hDLL;
	memset(module->name, 0, LABEL_SIZE);
	strncpy(module->name, mod_name, strlen(mod_name));

	*(void **)(&mod_init) = GetProcAddress(hDLL, "mod_init");
	(void)(*mod_init)();

	return NULL;

#endif

#ifdef linux
	Module *module = create_module();

	pid_t pid = fork();
	if (pid != 0) {
		module->pid = pid;
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
	Module *mod = modules[_modules_size - 1];
#ifdef  linux
	do {
		snprintf(message, sizeof(message), "Stopping module %s (pid: %d)", mod->name, mod->pid);
		debug(message, DEBUG_VERBOSE);
		kill(mod->pid, SIGKILL);
		free(mod);
		mod->hDLL = NULL;
		strcpy(mod->name, NULL);
	} while (--mod);
#endif 
#ifdef _WIN32
	do {
		snprintf(message, sizeof(message), "Stopping module %s (pid: %d)", mod->name, mod->hDLL);
		debug(message, DEBUG_VERBOSE);
		FreeLibrary(mod->hDLL);
		free(mod);
		mod->hDLL = NULL;
		strcpy(mod->name, NULL);
	} while (--mod);
#endif
}

