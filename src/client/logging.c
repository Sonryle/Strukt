#include <stdarg.h>

#include <client/logging.h>

FILE* client_log;

int init_client_log()
{
        client_log = fopen(LOG_FILENAME, "w"); 
        if (client_log == NULL)
        {
                fprintf(stderr, "Client Error - fopen(\"%s\", \"wa\"); failed...", LOG_FILENAME);
                return -1;
        }
        return 0;
}

void log_message(const char* level, const char* func, const char* file, const int line, const char* str, ...)
{
        va_list list;
        va_start(list, str);

        if (client_log)
        {
                fprintf(client_log, "%s : %s:%d: In function '%s': ", level, file, line, func);
                vfprintf(client_log, str, list);
                fprintf(client_log, "\n");
        }
        fprintf(stderr, "%s : %s:%d: In function '%s': ", level, file, line, func);
        vfprintf(stderr, str, list);
        fprintf(stderr, "\n");

        va_end(list);
        fflush(client_log);     // logs are flushed immediately, instead of waiting for file to close
}

void terminate_client_log()
{
        if (client_log)
                fclose(client_log);
}
