/*
 * File operation functions
*/
#pragma once
#include <stdbool.h>
#include <project_details.h>

/* returns shortened path relative to project root. Eg "/home/me/Documents/PROJECT_NAME/src/main.c" -> "/PROJECT_NAME/src/main.c" */
const char* truncate_path_to_project_root(const char* path);
/* returns 1 if file exists */
bool file_exists(const char* filename);
