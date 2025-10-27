#include <stdlib.h>

#include <errno.h>

#include <cwalk.h>

#include <main.h>
#include <logger.h>
#include <client/settings.h>
#include <client/window.h>
#include <client/renderer.h>

int init_app_context(struct AppContext* ctx);
int setup_project_root_path(struct AppPaths* paths);
int join_path(const char* path_name, const char* path1, const char* path2, const char* dest, int dest_size);
int setup_client_paths(struct AppPaths* paths);
int setup_server_paths(struct AppPaths* paths);
int init_subsystems(struct AppContext* ctx);
void terminate_app_context(struct AppContext* ctx);
void terminate_subsystems(struct AppContext* ctx);

struct AppContext app_context = { 0 };

int main()
{
    if (init_app_context(&app_context) != 0) {
        fprintf(stderr, "could not initiate app context\n");
        return -1;
    }

    while (!glfwWindowShouldClose(window))
    {
        temporary_render();

        /* Poll for and process events */
        glfwPollEvents();
    }

    terminate_app_context(&app_context);
    return 0;
}

int init_app_context(struct AppContext* ctx)
{
    if (setup_project_root_path(&ctx->paths) != 0) {
        fprintf(stderr, "idk\n");
        return -1;
    }
    if (setup_client_paths(&ctx->paths) != 0) {
        fprintf(stderr, "could not set up client paths for app context\n");
        return -1;
    }
    if (setup_server_paths(&ctx->paths) != 0) {
        fprintf(stderr, "could not set up server paths for app context\n");
        return -1;
    }
    if (init_subsystems(ctx) != 0) {
        fprintf(stderr, "Could not initialise subsystems for app context\n");
        return -1;
    }
    return 0;
}

int setup_project_root_path(struct AppPaths* paths) 
{
    // Get path to users home directory
#if defined (_WIN32)
    const char* home = getenv("APPDATA");
#else
    const char* home = getenv("HOME");
#endif
    if (home == NULL) {
        fprintf(stderr, "could not find home directory (getenv() returned NULL)\n");
        return -1;
    }

    // Create environment directories
#if defined (_WIN32)
    size_t size = cwk_path_join(home, PROJECT_NAME, paths->project_root_path, sizeof(paths->project_root_path));
#else
    size_t size = cwk_path_join(home, ".local/share/"PROJECT_NAME, paths->project_root_path, sizeof(paths->project_root_path));
#endif
    if (size >= sizeof(paths->project_root_path)) {
        fprintf(stderr, "%s home path too long (max %zu): %s\n", PROJECT_NAME, sizeof(paths->project_root_path), paths->project_root_path);
        return -1;
    }
    if (MKDIR(paths->project_root_path) != 0 && errno != EEXIST) {
        fprintf(stderr, "cannot create directory \"%s\": mkdir failed\n", paths->project_root_path);
        return -1;
    }

    return 0;
}

int join_path(const char* path_name, const char* path1, const char* path2, const char* dest, int dest_size)
{
    int size = cwk_path_join(path1, path2, dest, dest_size);
    if (size >= dest_size) {
        fprintf(stderr, "%s path too long (max %zu). Truncated path: %s\n", path_name, dest_size, dest);
        return -1;
    }
}

int setup_client_paths(struct AppPaths* paths)
{
    join_path("shaders",        paths->project_root_path,   CLIENT_SHADERS_DIRNAME,     paths->client_shaders_path, sizeof(paths->client_shaders_path));
    join_path("vertex shader",  paths->client_shaders_path, CLIENT_VSHADER_FILENAME,    paths->client_vshader_path, sizeof(paths->client_vshader_path));
    join_path("fragment shader",paths->client_shaders_path, CLIENT_FSHADER_FILENAME,    paths->client_fshader_path, sizeof(paths->client_fshader_path));
    join_path("client log",     paths->project_root_path,   CLIENT_LOG_FILENAME,        paths->client_log_path,     sizeof(paths->client_log_path));
    join_path("client settings",paths->project_root_path,   CLIENT_SETTINGS_FILENAME,   paths->client_settings_path,sizeof(paths->client_settings_path));
    if (MKDIR(paths->client_shaders_path) != 0 && errno != EEXIST) {
        fprintf(stderr, "cannot create directory \"%s\": mkdir failed\n", paths->client_shaders_path);
        return -1;
    }
    return 0;
}

int setup_server_paths(struct AppPaths* paths)
{
    join_path("server log", paths->project_root_path, SERVER_LOG_FILENAME, paths->server_log_path, sizeof(paths->server_log_path));
    return 0;
}

int init_subsystems(struct AppContext* ctx)
{
    // Initialise client environment
    if (init_logger(&ctx->client_logger, ctx->paths.client_log_path) != 0) {
        fprintf(stderr, "Could not create client logger\n");
        goto cleanup_client_log;
    }
    else {
        logger_log_message(&ctx->client_logger, LOG_INFO, "Welcome To The Client Log Of\n%s", PROJECT_NAME_ASCII_ART);
    }

    if (parse_client_settings(ctx->paths.client_settings_path) != 0) {
        logger_log_message(&ctx->client_logger, LOG_ERROR, "Could not initialise client settings");
        goto cleanup_client_settings;
    }
    if (init_window() != 0) {
        logger_log_message(&ctx->client_logger, LOG_ERROR, "Could not initialise window");
        goto cleanup_client_window;
    }
    if (init_renderer(ctx->paths.client_vshader_path, ctx->paths.client_fshader_path) != 0) {
        logger_log_message(&ctx->client_logger, LOG_ERROR, "Could not initialise renderer");
        goto cleanup_client_renderer;
    }

    // Initialise server environment
    if (init_logger(&ctx->server_logger, ctx->paths.server_log_path) != 0) {
        fprintf(stderr, "Could not create server logger\n");
        goto cleanup_server_log;
    }
    else {
        logger_log_message(&ctx->server_logger, LOG_INFO, "Welcome To The Server Log Of\n%s", PROJECT_NAME_ASCII_ART);
    }

    return 0;

cleanup_all:
    terminate_logger(&ctx->server_logger);
cleanup_server_log:
    terminate_renderer();
cleanup_client_renderer:
    terminate_window();
cleanup_client_window:
    terminate_settings();
cleanup_client_settings:
    terminate_logger(&ctx->client_logger);
cleanup_client_log:
    return -1;
}

void terminate_app_context(struct AppContext* ctx)
{
    terminate_subsystems(ctx);
    return;
}

void terminate_subsystems(struct AppContext* ctx)
{
    terminate_renderer();
    terminate_window();
    terminate_settings();
    terminate_logger(&ctx->client_logger);
    terminate_logger(&ctx->server_logger);
}
