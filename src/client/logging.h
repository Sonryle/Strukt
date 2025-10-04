/*
 * Provides functions for logging client information
*/
#pragma once

#include <stdio.h>
#include <string.h>

#include <strukt.h>

#define LOG_FILENAME "clog.txt"

#ifdef DEBUG_MODE
#define log_debug(fmt, ...) log_message("DEBUG", __FUNCTION__, file_path_relative_to_project(__FILE__), __LINE__, fmt, ##__VA_ARGS__)
#else
#define log_debug(fmt, ...) ((void)0)
#endif
#define log_err(fmt, ...) log_message("ERROR", __FUNCTION__, file_path_relative_to_project(__FILE__), __LINE__, fmt, ##__VA_ARGS__)
#define log_info(fmt, ...) log_message("INFO", __FUNCTION__, file_path_relative_to_project(__FILE__), __LINE__, fmt, ##__VA_ARGS__)
#define log_warn(fmt, ...) log_message("WARNING", __FUNCTION__, file_path_relative_to_project(__FILE__), __LINE__, fmt, ##__VA_ARGS__)


extern FILE *client_log;

int init_client_log();
void terminate_client_log();
void log_message(const char* level, const char* func, const char* file, const int line, const char* str, ...);