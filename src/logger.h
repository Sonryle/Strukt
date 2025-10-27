#pragma once

#define MAX_LOGS 10
#define LOG_LEVEL_DEFAULT LOG_INFO

typedef enum {
	LOG_DEBUG,
	LOG_INFO,
	LOG_WARNING,
	LOG_ERROR,
} LogLevel;

int logger_add_log(const char* log_path);
int logger_remove_log(int log_index);
void logger_set_log_level(int log_index, LogLevel level);
const char* log_level_to_string(LogLevel level);
void logger_log_message(int log_index, LogLevel level, const char* fmt, ...);
void terminate_logger();