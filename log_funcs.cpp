#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "log_funcs.h"

FILE* LOG_FILE = stderr;
char* EXTENSION = ".log";

void CloseLogFile()
{
    fprintf (LOG_FILE, "ЗАКРЫТ ФАЙЛ\n");
    fprintf (LOG_FILE, "\\----------------------------------------------------\\\n");
    fclose (LOG_FILE);
}

void OpenLogFile (const char* FILE_NAME)
{
    char* file_name = strdup (FILE_NAME);

    LOG_FILE = fopen (strcat(file_name, EXTENSION), "a");

    if (LOG_FILE == nullptr)
    {
        LOG_FILE = stderr;
    }

    fprintf (LOG_FILE, "\\----------------------------------------------------\\\n");
    fprintf (LOG_FILE, "ОТКРЫТ ФАЙЛ\n");
    atexit (CloseLogFile);
}

