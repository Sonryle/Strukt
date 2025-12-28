#pragma once
#include <stdio.h>

#define log_fatal(msg) log_message_to_file(LOG_FATAL, stdout, msg)
#define log_err(msg) log_message_to_file(LOG_ERROR, stdout, msg)
#define log_warn(msg) log_message_to_file(LOG_WARNING, stdout, msg)
#define log_info(msg) log_message_to_file(LOG_INFO, stdout, msg)
#define log_debug(msg) log_message_to_file(LOG_DEBUG, stdout, msg)

typedef enum {
    LOG_FATAL,
    LOG_ERROR,
    LOG_WARNING,
    LOG_INFO,
    LOG_DEBUG,
} LOG_LEVEL;

void log_message_to_file(LOG_LEVEL, FILE*, char*, ...);
