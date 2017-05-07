#define _CRT_SECURE_NO_WARNINGS
#ifndef H_SUSRAT_MOD
#define H_SUSRAT_MOD

#include <string.h>
#include <stdlib.h>
#include <stdio.h>


#ifdef _WIN32
#include <Windows.h>
#else
#ifdef linux
#include <sys/types.h>
#include <unistd.h>
#include <dlfcn.h>
#include <signal.h>
#endif
#endif


#include "debug.h"

#define NUM_MODULES 20
#define LABEL_SIZE 256
#define PATH_SIZE 500

typedef struct {
	char name[LABEL_SIZE];
#ifdef _WIN32
	HINSTANCE process;
#else
#ifdef linux
	pid_t process;
#endif
#endif
} Module;


#endif

Module *modules[NUM_MODULES];
extern int _modules_size;

