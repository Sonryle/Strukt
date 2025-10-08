#include <stdarg.h>
#include <stdlib.h>

#include <client/logging.h>

FILE* client_log;

int init_client_log(char* log_dir)
{
        // Create log path
        char* log_path = calloc(1, sizeof(char) * (strlen(log_dir) + strlen(LOG_FILENAME) + 1));
        strcat(log_path, log_dir);
        strcat(log_path, LOG_FILENAME);
        strcat(log_path, "");

        log_info("log_path = %s", log_path);
        client_log = fopen(log_path, "w"); 
        if (client_log == NULL)
        {
                fprintf(stderr, "Client Error - fopen(\"%s\", \"wa\"); failed...", LOG_FILENAME);
                return -1;
        }

        free(log_path);
        return 0;
}

void log_message(const char* level, const char* func, const char* file, const int line, const char* str, ...)
{
        va_list list;
        va_start(list, str);

#ifdef _DEBUG
        va_list copy;
        va_copy(copy, list);

        fprintf(stdout, "%s : %s:%d: In function '%s': ", level, file, line, func);
        vfprintf(stdout, str, copy);
        fprintf(stdout, "\n");

        va_end(copy);
#endif

        if (client_log)
        {
                fprintf(client_log, "%s : %s:%d: In function '%s': ", level, file, line, func);
                vfprintf(client_log, str, list);
                fprintf(client_log, "\n");
                fflush(client_log);     // logs are flushed immediately, instead of waiting for file to close
        }

        va_end(list);
}

void terminate_client_log()
{
        if (client_log)
                fclose(client_log);
}
