#include <string.h>
#include <stdio.h>
#include <file_operations.h>

const char* truncate_path_to_project_root(const char* path)
{
    const char* relative_path = strstr(path, PROJECT_NAME);
    return relative_path ? relative_path : path;
}

const char* get_project_dir()
{
        return (void*)0;
}

bool file_exists(const char* path)
{
    FILE* fp = fopen(path, "r");
    if (fp) fclose(fp);
    return fp != NULL;
}
