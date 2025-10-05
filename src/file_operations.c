#include <file_operations.h>

inline const char* file_path_relative_to_project(const char* path)
{
    const char* relative_path = strstr(path, PROJECT_NAME);
    return relative_path ? relative_path : path;
}

inline bool file_exists(const char* filename)
{
    FILE* fp = fopen(filename, "r");
    if (fp) fclose(fp);
    return fp != NULL;
}
