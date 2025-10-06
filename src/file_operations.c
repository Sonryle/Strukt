#include <file_operations.h>

const char* file_path_relative_to_project(const char* path)
{
    const char* relative_path = strstr(path, PROJECT_NAME);
    return relative_path ? relative_path : path;
}

bool file_exists(const char* path)
{
    FILE* fp = fopen(path, "r");
    if (fp) fclose(fp);
    return fp != NULL;
}
