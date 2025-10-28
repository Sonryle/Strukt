// There are two loggers, one for the client and one for the server
#pragma once

#define LOG_LEVEL_DEFAULT LOG_INFO

typedef enum {
	LOG_DEBUG,
	LOG_INFO,
	LOG_WARNING,
	LOG_ERROR,
} LogLevel;

typedef enum {
	CLIENT_LOG,
	SERVER_LOG
} LogIndex;

struct Log {
	FILE* fp;
	LogLevel log_level;
};

int init_logger(const char* client_log_path, const char* server_log_path);
void logger_set_log_level(LogIndex logger, LogLevel level);
const char* log_level_to_string(LogLevel level);
void logger_log_message(LogIndex logger, LogLevel level, const char* fmt, ...);
void terminate_logger();