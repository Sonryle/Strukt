#if defined (_WIN32)
#include <direct.h>
#else
#include <sys/stat.h>
#endif
#include <stdlib.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>

#include <cwalk.h>

#include <main.h>
#include <ASCIIart.h>
#include <logger.h>
#include <settings.h>
#include <client/window.h>
#include <client/renderer.h>

/* This is neccessary as we need to be able to log */
/* errors even before the logger is initialised. */
#define log_early_error(fmt, ...) fprintf(stderr, fmt,  ##__VA_ARGS__)

int setup_path(const char* path_name, const char* path1, const char* path2, const char* dest, int dest_size);
int make_directory(const char* path);
int get_project_root_dir(struct AppPaths* paths);
int setup_paths(struct AppPaths* paths);
int init_subsystems(struct AppState* app_state);
void terminate_app_context(struct AppState* app_state);

int main()
{
    struct AppState app_context = { 0 };
    if (setup_paths(&app_context.paths) != 0) {
        log_early_error("could not set up paths for app context");
        return -1;
    }
    if (init_subsystems(&app_context) != 0) {
        log_early_error("Could not initialise subsystems for app context");
        return -1;
    }

    while (!glfwWindowShouldClose(app_context.graphics.window))
    {
        temporary_render(&app_context.graphics.VAO, &app_context.graphics.shader_program, app_context.graphics.window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    terminate_app_context(&app_context);
    return 0;
}

int setup_path(const char* path_name, const char* path1, const char* path2, const char* dest, int dest_size)
{
    int size = cwk_path_join(path1, path2, dest, dest_size);
        if (size >= dest_size) {
        log_early_error("%s path too long (max %d). Truncated path: %s", path_name, dest_size, dest);
        return -1;
    }
    return 0;
}

int make_directory(const char* path)
{
#if defined (_WIN32)
    int result = _mkdir(path);
#else
    int result = mkdir(path, 0755);
#endif
    if (result != 0 && errno != EEXIST) {
        log_early_error("cannot create directory \"%s\": mkdir failed", path);
        return -1;
    }
    return 0;
}

int get_project_root_dir(struct AppPaths* paths)
{
#if defined (_WIN32)
    const char* home = getenv("APPDATA");
    const char* subpath = PROJECT_NAME;
#else
    const char* home = getenv("HOME");
    const char* subpath = ".local/share/"PROJECT_NAME;
#endif
    if (home == NULL) {
        log_early_error("could not find home directory (getenv() returned NULL)");
        return -1;
    }
    return setup_path(PROJECT_NAME" root", home, subpath, paths->project_root_path, sizeof(paths->project_root_path));
}

int setup_paths(struct AppPaths* paths)
{
    // Project Root Path
    if (get_project_root_dir(paths) != 0)
        return -1;

    // Client paths
    if (setup_path("client shaders", paths->project_root_path, CLIENT_SHADERS_DIRNAME, paths->client_shaders_path, sizeof(paths->client_shaders_path)) != 0) return -1;
    if (setup_path("client vertex shader", paths->client_shaders_path, CLIENT_VSHADER_FILENAME, paths->client_vshader_path, sizeof(paths->client_vshader_path)) != 0) return -1;
    if (setup_path("client fragment shader", paths->client_shaders_path, CLIENT_FSHADER_FILENAME, paths->client_fshader_path, sizeof(paths->client_fshader_path)) != 0) return -1;
    if (setup_path("client log", paths->project_root_path, CLIENT_LOG_FILENAME, paths->client_log_path, sizeof(paths->client_log_path)) != 0) return -1;
    if (setup_path("client settings", paths->project_root_path, CLIENT_SETTINGS_FILENAME, paths->client_settings_path, sizeof(paths->client_settings_path)) != 0) return -1;

    // Server Paths
    if (setup_path("server log", paths->project_root_path, SERVER_LOG_FILENAME, paths->server_log_path, sizeof(paths->server_log_path)) != 0) return -1;
    if (setup_path("server settings", paths->project_root_path, SERVER_SETTINGS_FILENAME, paths->server_settings_path, sizeof(paths->server_settings_path)) != 0) return -1;

    // Create required directories
    if (make_directory(paths->project_root_path) != 0) return -1;
    if (make_directory(paths->client_shaders_path) != 0) return -1;

    return 0;
}

int init_subsystems(struct AppState* app_state)
{
    // Initialise global environment
    int err = init_logger(app_state->paths.client_log_path,
                        app_state->paths.server_log_path);
    if (err != 0) { log_early_error("Could not initialise logger"); goto failure_logger; }

    // Initialise client environment
    err = init_settings(&app_state->client_settings,
                        &app_state->server_settings,
                        app_state->paths.client_settings_path,
                        app_state->paths.server_settings_path);
    if (err != 0) { logger_log_message(CLIENT_LOG, LOG_ERROR, "Could not initialise client settings"); goto failure_client_settings; }
    logger_log_message(CLIENT_LOG, LOG_INFO, "Settings successfully initiated & parsed");

    err = init_window(&app_state->graphics.window,
                    app_state->client_settings.window.title,
                    app_state->client_settings.window.initial_width,
                    app_state->client_settings.window.initial_height,
                    app_state->client_settings.window.fullscreen);
    if (err != 0) { logger_log_message(CLIENT_LOG, LOG_ERROR, "Could not initialise window"); goto failure_client_window; }
    logger_log_message(CLIENT_LOG, LOG_INFO, "Client window successfully initiated");

    err = init_renderer(&app_state->graphics.VBO, &app_state->graphics.VAO, &app_state->graphics.shader_program,
                        app_state->paths.client_vshader_path, app_state->paths.client_fshader_path,
                        &app_state->client_settings.renderer);
    if (err != 0) { logger_log_message(CLIENT_LOG, LOG_ERROR, "Could not initialise renderer"); goto failure_client_renderer; }
    logger_log_message(CLIENT_LOG, LOG_INFO, "Client renderer successfully initiated");

    // Initialise server environment
    // Nothing to do here yet.

    // Set log levels after 
    logger_set_log_level(CLIENT_LOG, app_state->client_settings.log_level);
    logger_set_log_level(SERVER_LOG, app_state->server_settings.log_level);
    return 0;

failure_client_renderer:
    terminate_window(app_state->graphics.window);
failure_client_window:
    terminate_settings();
failure_client_settings:
    terminate_logger();
failure_logger:
    return -1;
}

void terminate_app_context(struct AppState* app_state)
{
    logger_log_message(CLIENT_LOG, LOG_INFO, "%s Session Terminated. Goodbye!\n%s\n", PROJECT_NAME, ASCII_TEXT_DIVIDER);
    logger_log_message(SERVER_LOG, LOG_INFO, "%s Session Terminated. Goodbye!\n%s\n", PROJECT_NAME, ASCII_TEXT_DIVIDER);
    terminate_renderer(&app_state->graphics.shader_program);
    terminate_window(app_state->graphics.window);
    terminate_settings();
    terminate_logger();
}
