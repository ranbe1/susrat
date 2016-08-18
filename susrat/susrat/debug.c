#include "debug.h"

void
debug(char *message, DebugLevel type) {
	int pipe;
	char buffer[MAX_DEBUG_MESSAGE];
	char message_type[6];
	memset(message_type, 0, 6);

	pipe = 1;

	if (type == DEBUG_VERBOSE) {
		strcpy(message_type, "DEBUG");
	}
	else if (debug_level & type) {
		if (type == DEBUG_INFO) {
			strcpy(message_type, "INFO");
		}
		else if (type == DEBUG_WARNING) {
			strcpy(message_type, "WARN");
			pipe = 2;
		}
		else if (type == DEBUG_ERROR) {
			strcpy(message_type, "ERR");
			pipe = 2;
		}
	}

	int message_size = strlen(message_type) + strlen(message) + strlen(": \n") + 1;
	snprintf(buffer, message_size, "%s: %s\n", message_type, message);
	write(pipe, buffer, message_size);
}

