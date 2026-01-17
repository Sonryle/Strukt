#include <stdio.h>
#include <stdarg.h>

#include "Strukt/logger.h"

static char* log_level_to_string(LOG_LEVEL lvl)
{
    switch (lvl) {
        case LOG_FATAL:
            return "FATAL";
        case LOG_ERROR:
            return "ERROR";
        case LOG_WARNING:
            return "WARNING";
        case LOG_INFO:
            return "INFO";
        case LOG_DEBUG:
            return "DEBUG";
    }
}

void log_message_to_file(LOG_LEVEL lvl, FILE* fp, char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    fprintf(fp, "%s: ", log_level_to_string(lvl));
    vfprintf(fp, fmt, args);
    fprintf(fp, "\n");
    va_end(args);
}
