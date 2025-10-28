#pragma once

#include <main.h>
#include <logger.h>

#define log_err(fmt, ...) logger_log_message(&app_context.client_logger, LOG_ERROR, fmt, ##__VA_ARGS__)
#define log_info(fmt, ...) logger_log_message(&app_context.client_logger, LOG_INFO, fmt, ##__VA_ARGS__)
#define log_warn(fmt, ...) logger_log_message(&app_context.client_logger, LOG_WARNING, fmt, ##__VA_ARGS__)
#define log_debug(fmt, ...) logger_log_message(&app_context.client_logger, LOG_DEBUG, fmt, ##__VA_ARGS__)
