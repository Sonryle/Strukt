#include <stdlib.h>

#include <errno.h>

#include <cwalk.h>

#include <ASCIIart.h>
#include <main.h>
#include <logger.h>
#include <client/settings.h>
#include <client/window.h>
#include <client/renderer.h>

int init_app_context(struct AppContext* ctx);
int join_path(const char* path_name, const char* path1, const char* path2, const char* dest, int dest_size);
int setup_paths(struct AppPaths* paths);
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
        temporary_render(&app_context.VAO, &app_context.shader_program);

        /* Poll for and process events */
        glfwPollEvents();
    }

    terminate_app_context(&app_context);
    return 0;
}

int init_app_context(struct AppContext* ctx)
{
    if (setup_paths(&ctx->paths) != 0) {
        fprintf(stderr, "could not set up paths for app context\n");
        return -1;
    }
    if (init_subsystems(ctx) != 0) {
        fprintf(stderr, "Could not initialise subsystems for app context\n");
        return -1;
    }
    return 0;
}

int join_path(const char* path_name, const char* path1, const char* path2, const char* dest, int dest_size)
{
    int size = cwk_path_join(path1, path2, dest, dest_size);
    if (size >= dest_size) {
        fprintf(stderr, "%s path too long (max %d). Truncated path: %s\n", path_name, dest_size, dest);
        return -1;
    }
}

int setup_paths(struct AppPaths* paths)
{
    // Project Root Path
#if defined (_WIN32)
    const char* home = getenv("APPDATA");
    join_path("home", home, PROJECT_NAME, paths->project_root_path, sizeof(paths->project_root_path));
#else
    const char* home = getenv("HOME");
    join_path("home", home, ".local/share/"PROJECT_NAME, paths->project_root_path, sizeof(paths->project_root_path));
#endif
    if (home == NULL) {
        fprintf(stderr, "could not find home directory (getenv() returned NULL)\n");
        return -1;
    }
    if (MKDIR(paths->project_root_path) != 0 && errno != EEXIST) {
        fprintf(stderr, "cannot create directory \"%s\": mkdir failed\n", paths->project_root_path);
        return -1;
    }

    // Client paths
    join_path("shaders", paths->project_root_path, CLIENT_SHADERS_DIRNAME, paths->client_shaders_path, sizeof(paths->client_shaders_path));
    join_path("vertex shader", paths->client_shaders_path, CLIENT_VSHADER_FILENAME, paths->client_vshader_path, sizeof(paths->client_vshader_path));
    join_path("fragment shader", paths->client_shaders_path, CLIENT_FSHADER_FILENAME, paths->client_fshader_path, sizeof(paths->client_fshader_path));
    join_path("client log", paths->project_root_path, CLIENT_LOG_FILENAME, paths->client_log_path, sizeof(paths->client_log_path));
    join_path("client settings", paths->project_root_path, CLIENT_SETTINGS_FILENAME, paths->client_settings_path, sizeof(paths->client_settings_path));
    if (MKDIR(paths->client_shaders_path) != 0 && errno != EEXIST) {
        fprintf(stderr, "cannot create directory \"%s\": mkdir failed\n", paths->client_shaders_path);
        return -1;
    }

    // Server Paths
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
        logger_log_message(&ctx->client_logger, LOG_INFO, "Welcome To The Client Log Of\n%s", PROJECT_NAME_ASCII_ART5);
    }

    if (parse_client_settings(ctx->paths.client_settings_path) != 0) {
        logger_log_message(&ctx->client_logger, LOG_ERROR, "Could not initialise client settings");
        goto cleanup_client_settings;
    }
    if (init_window() != 0) {
        logger_log_message(&ctx->client_logger, LOG_ERROR, "Could not initialise window");
        goto cleanup_client_window;
    }
    if (init_renderer(&ctx->VBO, &ctx->VAO, &ctx->shader_program, ctx->paths.client_vshader_path, ctx->paths.client_fshader_path) != 0) {
        logger_log_message(&ctx->client_logger, LOG_ERROR, "Could not initialise renderer");
        goto cleanup_client_renderer;
    }
    // Initialise server environment
    if (init_logger(&ctx->server_logger, ctx->paths.server_log_path) != 0) {
        fprintf(stderr, "Could not create server logger\n");
        goto cleanup_server_log;
    }
    else {
        logger_log_message(&ctx->server_logger, LOG_INFO, "Welcome To The Server Log Of\n%s", PROJECT_NAME_ASCII_ART2);
    }

    return 0;

cleanup_all:
    terminate_logger(&ctx->server_logger);
cleanup_server_log:
    terminate_renderer(&ctx->shader_program);
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
    terminate_renderer(&ctx->shader_program);
    terminate_window();
    terminate_settings();
    terminate_logger(&ctx->client_logger);
    terminate_logger(&ctx->server_logger);
}
