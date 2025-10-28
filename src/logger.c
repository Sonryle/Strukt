#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

#include "logger.h"

int init_logger(struct Logger* logger, const char* log_path)
{
    if (log_path == NULL) {
        fprintf(stderr, "Cannot add log with NULL log path\n");
        return -1;
    }

    logger->fp = fopen(log_path, "w");
    logger->log_level = LOG_LEVEL_DEFAULT;

    if (logger->fp == NULL) {
        fprintf(stderr, "Logger cannot open file (%s) for writing\n", log_path);
        return -1;
    }

    return 0;
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

void logger_set_log_level(struct Logger* logger, LogLevel level)
{
    logger->log_level = level;
}

void logger_log_message(struct Logger* logger, LogLevel level, const char* fmt, ...)
{
    if (logger == NULL) {
        printf("Cannot log to NULL logger pointer\n");
        return;
    }
    if (level < logger->log_level)
        return;

    if (logger->fp) {
        fprintf(logger->fp, "%s : ", log_level_to_string(level));

        va_list list;
        va_start(list, fmt);
        vfprintf(logger->fp, fmt, list);
        va_end(list);

        fprintf(logger->fp, "\n");
        fflush(logger->fp);
    }
    else {
        fprintf(stderr, "Cannot log to uninitialised logger\n");
    }
}

void terminate_logger(struct Logger* logger)
{
    if (logger->fp != NULL)
        fclose(logger->fp);
    logger->fp == NULL;
}
