#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

#include "logger.h"

struct Log {
    FILE* fp;
    LogLevel log_level;
    int index;
};
static struct Log logs[MAX_LOGS] = { NULL };

int logger_add_log(const char* log_path)
{
    if (log_path == NULL) {
        fprintf(stderr, "cannot add log with NULL log path\n");
        return -1;
    }

    /* Find empty log struct in logs array */
    for (int n = 0; n < MAX_LOGS; n++) {
        if (logs[n].fp != NULL)
            continue;

        logs[n].fp = fopen(log_path, "a");
        logs[n].log_level = LOG_LEVEL_DEFAULT;
        logs[n].index = n;

        if (logs[n].fp == NULL) {
            fprintf(stderr, "logger cannot open file %s for writing", log_path);
            return -1;
        }

        // Return index of log (its position in the logs array)
        return n;
    }

    // If the program makes it here, then we could
    // not find empty place for new log in logs array
    fprintf(stderr, "cannot add more than (%d) logs to logger.", MAX_LOGS);
    return -1;
}

int logger_remove_log(int log_index)
{
    if (log_index > MAX_LOGS || log_index < 0) {
        fprintf(stderr, "cannot remove log with index (%d). index is invalid (either above %d or below 0)", log_index, MAX_LOGS);
        return -1;
    }

    if (logs[log_index].fp != NULL)
        fclose(logs[log_index].fp);
    logs[log_index].fp = NULL;
    logs[log_index].log_level = LOG_LEVEL_DEFAULT;
    logs[log_index].index = -1;
}

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

void logger_set_log_level(int log_index, LogLevel level)
{
    logs[log_index].log_level = level;
}

void logger_log_message(int log_index, LogLevel level, const char* fmt, ...)
{
    if (level < logs[log_index].log_level)
        return;

    if (logs[log_index].fp) {
        fprintf(logs[log_index].fp, "%s : ", log_level_to_string(level));

        va_list list;
        va_start(list, fmt);
        vfprintf(logs[log_index].fp, fmt, list);
        va_end(list);

        fprintf(logs[log_index].fp, "\n");
        fflush(logs[log_index].fp);
    }
    else {
        fprintf(stderr, "cannot print message to log of index (%d). No log exists at this location\n", log_index);
    }
}

void terminate_logger()
{
    for (int n = 0; n < MAX_LOGS; n++)
        logger_remove_log(n);
}
