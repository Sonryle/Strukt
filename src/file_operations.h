/*
 * File operation functions
*/
#pragma once
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <strukt.h>

const char* file_path_relative_to_project(const char* path);

bool file_exists(const char* filename);
