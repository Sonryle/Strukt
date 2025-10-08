#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include <project_details.h>

char* project_data_dir;

void find_project_data_dir();

int fetch_project_details()
{
        find_project_data_dir();
        return 0;
}

void find_project_data_dir()
{
#if defined(_WIN32)
        /* Find project data dir */
        const char* home = getenv("APPDATA");
        const char* rest = "\\";
        project_data_dir = calloc(1, sizeof(char) * (strlen(home) + strlen(rest) + strlen(PROJECT_NAME) + 2));
        if (project_data_dir != NULL)
        {
                strcat(project_data_dir, home);
                strcat(project_data_dir, rest);
                strcat(project_data_dir, PROJECT_NAME);
                strcat(project_data_dir, "\\");
        }
        
        mkdir(project_data_dir, 0777);
#else
        const char* home = getenv("HOME");
        const char* rest = "/.local/share/";
        project_data_dir = calloc(1, sizeof(char) * (strlen(home) + strlen(rest) + strlen(PROJECT_NAME) + 2));
        if (project_data_dir != NULL)
        {
                strcat(project_data_dir, home);
                strcat(project_data_dir, rest);
                strcat(project_data_dir, PROJECT_NAME);
                strcat(project_data_dir, "/");
        }

        mkdir(project_data_dir, 0777);
#endif
}

void terminate_project_details()
{
    free(project_data_dir);
}
