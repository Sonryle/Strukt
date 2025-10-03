/*
 * Provides functions for logging client information
*/
#pragma once

#define STRUKT_CLIENT_LOG_FILENAME "client_log.txt"
#define __RELATIVE_FILE__ strstr(__FILE__, "Strukt")    // Returns __FILE__ path relative to Strukt directory.
#define log_debug(fmt, ...) pre_macro_log_debug(__FUNCTION__, __RELATIVE_FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define log_err(fmt, ...) pre_macro_log_err(__FUNCTION__, __RELATIVE_FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define log_info(fmt, ...) pre_macro_log_info(__FUNCTION__, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define log_warn(fmt, ...) pre_macro_log_warn(__FUNCTION__, __FILE__, __LINE__, fmt, ##__VA_ARGS__)

#include <stdio.h>
#include <string.h>

extern FILE *client_log;

int init_client_log();
void terminate_client_log();
void pre_macro_log_debug(const char* func, const char* file, const int line, const char* s, ...);
void pre_macro_log_err(const char* func, const char* file, const int line, const char* s, ...);
void pre_macro_log_info(const char* func, const char* file, const int line, const char* s, ...);
void pre_macro_log_warn(const char* func, const char* file, const int line, const char* s, ...);
