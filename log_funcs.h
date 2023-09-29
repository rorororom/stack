#ifndef LOG_FUNCS
#define LOG_FUNCS

#include <stdio.h>

extern FILE* LOG_FILE;

void CloseLogFile();
void OpenLogFile (const char* FILE_NAME, const char* mode);

#endif
