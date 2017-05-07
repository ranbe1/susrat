#define _CRT_SECURE_NO_WARNINGS
#ifndef H_SUSRAT_DEBUG
#define H_SUSRAT_DEBUG

#include <stdio.h>
#include <string.h>

#ifdef  linux
#include <unistd.h>
#endif 

#define MAX_DEBUG_MESSAGE 1024

typedef enum {
	DEBUG_VERBOSE = 1,
	DEBUG_ERROR = 2,
	DEBUG_WARNING = 4,
	DEBUG_INFO = 8
} DebugLevel;

extern DebugLevel debug_level;

#endif