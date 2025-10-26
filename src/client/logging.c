#include "logging.h"
#include <stdarg.h>
#include <stdio.h>

static FILE* log_file = NULL;
static LogLevel current_log_level = LOG_INFO;

inline const char* log_level_to_string(LogLevel level)
{
    switch (level) {
    case LOG_DEBUG:     return "DEBUG";
    case LOG_INFO:      return "INFO";
    case LOG_WARNING:   return "WARNING";
    case LOG_ERROR:     return "ERROR";
    default:            return "UNKNOWN";
    }
}

int init_logger(char* log_path)
{
        if (log_file != NULL)
            return -1;

        /* Open Log File */
        log_file = fopen(log_path, "w");
        if (log_file == NULL) {
                fprintf(stderr, "Client Error - fopen(\"%s\", \"w\")\n", log_path);
                return -1;
        }

        return 0;
}

void set_log_level(LogLevel level)
{
    current_log_level = level;
}

void log_message(LogLevel level, const char* fmt, ...)
{
    if (level < current_log_level)
        return;

    if (log_file) {
        fprintf(log_file, "%s : ", log_level_to_string(level));

        va_list list;
        va_start(list, fmt);
        vfprintf(log_file, fmt, list);
        va_end(list);

        fprintf(log_file, "\n");
        fflush(log_file);
    }
}

void terminate_logger()
{
    if (log_file)
        fclose(log_file);
    log_file = NULL;
}
