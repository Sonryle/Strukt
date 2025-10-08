/*
 * Provides functions for logging client information
*/
#pragma once
#include <stdio.h>
#include <string.h>

#include <file_operations.h>

#define LOG_FILENAME "clog.txt"
#define log_err(fmt, ...) log_message("ERROR", __FUNCTION__, truncate_path_to_project_root(__FILE__), __LINE__, fmt, ##__VA_ARGS__)
#define log_info(fmt, ...) log_message("INFO", __FUNCTION__, truncate_path_to_project_root(__FILE__), __LINE__, fmt, ##__VA_ARGS__)
#define log_warn(fmt, ...) log_message("WARNING", __FUNCTION__, truncate_path_to_project_root(__FILE__), __LINE__, fmt, ##__VA_ARGS__)
#ifdef _DEBUG
#define log_debug(fmt, ...) log_message("DEBUG", __FUNCTION__, truncate_path_to_project_root(__FILE__), __LINE__, fmt, ##__VA_ARGS__)
#else
#define log_debug(fmt, ...) ((void)0)
#endif

int init_client_log(char* log_dir);
void terminate_client_log();
void log_message(const char* level, const char* func, const char* file, const int line, const char* str, ...);
extern FILE *client_log;
