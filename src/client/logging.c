#include <stdarg.h>

#include <client/logging.h>

FILE* client_log;

int init_client_log()
{
        client_log = fopen(LOG_FILENAME, "wa"); 
        if (client_log == NULL)
        {
                fprintf(stderr, "Client Error - fopen(\"%s\", \"wa\"); failed...", LOG_FILENAME);
                return -1;
        }
        return 0;
}

void log_debug(const char* func, const char* file, const int line, const char* s, ...)
{
// #ifdef DEBUG
        if (client_log)
        {
                va_list list;
                va_start(list, s);

                fprintf(client_log, "%s:%d: In function '%s': DEBUG : ", file, line, func);
                vfprintf(client_log, s, list);
                fprintf(client_log, "\n");
                fprintf(stderr, "%s:%d: In function '%s': DEBUG : ", file, line, func);    // Debug logs are printed to stderr (useful to see)
                vfprintf(stderr, s, list);
                fprintf(stderr, "\n");

                va_end(list);
                fflush(client_log);     // Important logs are flushed immediately
        }
// #endif
}

void log_err(const char* func, const char* file, const int line, const char* s, ...)
{
        if (client_log)
        {
                va_list list;
                va_start(list, s);

                fprintf(client_log, "%s:%d: In function '%s': ERROR : ", file, line, func);
                vfprintf(client_log, s, list);
                fprintf(client_log, "\n");
                fprintf(stderr, "%s:%d: In function '%s': ERROR : ", file, line, func);         // Error logs are printed to stderr (cannot be ignored)
                vfprintf(stderr, s, list);
                fprintf(stderr, "\n");

                va_end(list);
                fflush(client_log);     // Errors are flushed immediately
        }
}

void log_info(const char* func, const char* file, const int line, const char* s, ...)
{
        if (client_log)
        {
                va_list list;
                va_start(list, s);

                fprintf(client_log, "%s:%d: In function '%s': INFO : ", file, line, func);      // Error logs are printed to stderr (cannot be ignored)
                vfprintf(client_log, s, list);
                fprintf(client_log, "\n");

                va_end(list);
        }
}

void log_warn(const char* func, const char* file, const int line, const char* s, ...)
{
        if (client_log)
        {
                va_list list;
                va_start(list, s);

                fprintf(client_log, "%s:%d: In function '%s': WARNING : ", file, line, func);   // Error logs are printed to stderr (cannot be ignored)
                vfprintf(client_log, s, list);
                fprintf(client_log, "\n");

                va_end(list);
        }
}

void terminate_client_log()
{
        if (client_log)
                fclose(client_log);
}
