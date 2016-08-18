#ifndef  _MOD_
#define _MOD_

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "debug.h"

#define NUM_MODULES 20
#define LABEL_SIZE 256
#define PATH_SIZE 500

#ifdef _WIN32

#include <Windows.h>

typedef struct {
	char name[LABEL_SIZE];
	HINSTANCE hDLL;
} Module;

#endif

#ifdef linux

#include <sys/types.h>
#include <unistd.h>
#include <dlfcn.h>
#include <signal.h>

typedef struct {
	char name[LABEL_SIZE];
	pid_t pid;
} Module;

#endif

Module *modules[NUM_MODULES];
extern int _modules_size;

#endif