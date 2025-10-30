#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

#include "logger.h"
#include "ASCIIart.h"

static struct Log logs[2] = {0};

int init_logger(const char* client_log_path, const char* server_log_path)
{
    if (client_log_path == NULL || server_log_path == NULL) {
        fprintf(stderr, "Logger cannot be initiated with NULL log path\n");
        return -1;
    }

    logs[CLIENT_LOG].fp = fopen(client_log_path, "a");
    logs[SERVER_LOG].fp = fopen(server_log_path, "a");
    logs[CLIENT_LOG].log_level = LOG_LEVEL_DEFAULT;
    logs[SERVER_LOG].log_level = LOG_LEVEL_DEFAULT;
    logger_log_message(CLIENT_LOG, LOG_INFO, "Client log successfully initiated, Welcome to the Client Log of\n%s", PROJECT_NAME_ASCII_ART5);
    logger_log_message(SERVER_LOG, LOG_INFO, "Server log successfully initiated, Welcome to the Server Log of\n%s", PROJECT_NAME_ASCII_ART4);

    if (logs[CLIENT_LOG].fp == NULL) {
        fprintf(stderr, "Logger cannot open file (%s) for writing\n", client_log_path);
        return -1;
    }
    if (logs[SERVER_LOG].fp == NULL) {
        fprintf(stderr, "Logger cannot open file (%s) for writing\n", server_log_path);
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

void logger_set_log_level(LogIndex index, LogLevel level)
{
    logs[index].log_level = level;
    fprintf(logs[index].fp, "Logger : Log Level changed to \"%s\"\n", log_level_to_string(level));
}

void logger_log_message(LogIndex index, LogLevel level, const char* fmt, ...)
{
    if (index != CLIENT_LOG && index != SERVER_LOG) {
        printf("Cannot print to log of index (%d)\n", index);
        return;
    }
    if (level < logs[index].log_level)
        return;

    if (logs[index].fp) {
        fprintf(logs[index].fp, "%s : ", log_level_to_string(level));

        va_list list;
        va_start(list, fmt);
        vfprintf(logs[index].fp, fmt, list);
        va_end(list);

        fprintf(logs[index].fp, "\n");
        fflush(logs[index].fp);
    }
    else {
        fprintf(stderr, "Cannot log to uninitialised logger\n");
    }
}

void terminate_logger()
{
    if (logs[CLIENT_LOG].fp != NULL)
        fclose(logs[CLIENT_LOG].fp);
    if (logs[SERVER_LOG].fp != NULL)
        fclose(logs[SERVER_LOG].fp);
    logs[CLIENT_LOG].fp == NULL;
    logs[SERVER_LOG].fp == NULL;
}
