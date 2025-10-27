#pragma once

#include <main.h>

#define log_err(fmt, ...) logger_log_message(client_log_index, LOG_ERROR, fmt, ##__VA_ARGS__)
#define log_info(fmt, ...) logger_log_message(client_log_index, LOG_INFO, fmt, ##__VA_ARGS__)
#define log_warn(fmt, ...) logger_log_message(client_log_index, LOG_WARNING, fmt, ##__VA_ARGS__)
#define log_debug(fmt, ...) logger_log_message(client_log_index, LOG_DEBUG, fmt, ##__VA_ARGS__)
