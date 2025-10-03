/*
 * Provides functions for logging client information
*/
#pragma once

#include <stdio.h>
#include <string.h>

#include <strukt.h>

#define LOG_FILENAME "clog.txt"
#define LOG_DEBUG(fmt, ...) log_debug(__FUNCTION__, file_path_relative_to_project(__FILE__), __LINE__, fmt, ##__VA_ARGS__)
#define LOG_ERR(fmt, ...) log_err(__FUNCTION__, file_path_relative_to_project(__FILE__), __LINE__, fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...) log_info(__FUNCTION__, file_path_relative_to_project(__FILE__), __LINE__, fmt, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...) log_warn(__FUNCTION__, file_path_relative_to_project(__FILE__), __LINE__, fmt, ##__VA_ARGS__)

extern FILE *client_log;

int init_client_log();
void terminate_client_log();
void log_debug(const char* func, const char* file, const int line, const char* s, ...);
void log_err(const char* func, const char* file, const int line, const char* s, ...);
void log_info(const char* func, const char* file, const int line, const char* s, ...);
void log_warn(const char* func, const char* file, const int line, const char* s, ...);
