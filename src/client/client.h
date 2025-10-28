#pragma once

#include <main.h>
#include <logger.h>

#define log_err(fmt, ...) logger_log_message(CLIENT_LOG, LOG_ERROR, fmt, ##__VA_ARGS__)
#define log_info(fmt, ...) logger_log_message(CLIENT_LOG, LOG_INFO, fmt, ##__VA_ARGS__)
#define log_warn(fmt, ...) logger_log_message(CLIENT_LOG, LOG_WARNING, fmt, ##__VA_ARGS__)
#define log_debug(fmt, ...) logger_log_message(CLIENT_LOG, LOG_DEBUG, fmt, ##__VA_ARGS__)
