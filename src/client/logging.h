#pragma once

#define log_err(fmt, ...) log_message(LOG_ERROR, fmt, ##__VA_ARGS__)
#define log_info(fmt, ...) log_message(LOG_INFO, fmt, ##__VA_ARGS__)
#define log_warn(fmt, ...) log_message(LOG_WARNING, fmt, ##__VA_ARGS__)
#define log_debug(fmt, ...) log_message(LOG_DEBUG, fmt, ##__VA_ARGS__)

typedef enum {
	LOG_DEBUG,
	LOG_INFO,
	LOG_WARNING,
	LOG_ERROR,
} LogLevel;

int init_logger(char* log_dir);
void set_log_level(LogLevel level);
const char* log_level_to_string(LogLevel level);
void log_message(LogLevel level, const char* fmt, ...);
void terminate_logger();