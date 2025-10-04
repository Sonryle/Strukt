/*
 * Responsible for functions used across entire Strukt scope
*/
#pragma once

#define PROJECT_NAME "Strukt"
inline char* file_path_relative_to_project(const char* path)
{
    const char* relative_path = strstr(path, PROJECT_NAME);
    return relative_path ? relative_path : path;
}
