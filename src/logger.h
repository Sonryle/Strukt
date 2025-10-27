#pragma once

#define MAX_LOGS 10
#define LOG_LEVEL_DEFAULT LOG_INFO

typedef enum {
	LOG_DEBUG,
	LOG_INFO,
	LOG_WARNING,
	LOG_ERROR,
} LogLevel;

struct Logger {
	FILE* fp;
	LogLevel log_level;
};

int init_logger(struct Logger* logger, const char* log_path);
void logger_set_log_level(struct Logger* logger, LogLevel level);
const char* log_level_to_string(LogLevel level);
void logger_log_message(struct Logger* logger, LogLevel level, const char* fmt, ...);
void terminate_logger(struct Logger* logger);