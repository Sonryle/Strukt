#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include <GLFW/glfw3.h>
#include <cwalk.h>

#include <main.h>
#include <logger.h>
#include <client/settings.h>
#include <client/window.h>
#include <client/renderer.h>

int setup_paths();
int init_subsystems();
void terminate_subsystems();

static char project_root_path[FILENAME_MAX];

static char client_shaders_path[FILENAME_MAX];
static char client_vshader_path[FILENAME_MAX];
static char client_fshader_path[FILENAME_MAX];
static char client_log_path[FILENAME_MAX];
static char client_settings_path[FILENAME_MAX];

static char server_log_path[FILENAME_MAX];

int client_log_index;
int server_log_index;

int main()
{
    if (setup_paths() != 0)
        return -1;
    if (init_subsystems() != 0)
        return -1;

    while (!glfwWindowShouldClose(window))
    {
        temporary_render();

        /* Poll for and process events */
        glfwPollEvents();
    }

    terminate_subsystems();
    return 0;
}

int setup_paths()
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
    size_t size = cwk_path_join(home, PROJECT_NAME, project_root_path, sizeof(project_root_path));
#else
    size_t size = cwk_path_join(home, ".local/share/"PROJECT_NAME, project_root_path, sizeof(project_root_path));
#endif
    if (size >= sizeof(project_root_path)) {
        fprintf(stderr, "%s home path too long (max %zu): %s\n", PROJECT_NAME, sizeof(project_root_path), project_root_path);
        return -1;
    }
    if (MKDIR(project_root_path) != 0 && errno != EEXIST) {
        fprintf(stderr, "cannot create directory \"%s\": mkdir failed\n", project_root_path);
        return -1;
    }

    size = cwk_path_join(project_root_path, CLIENT_SHADERS_DIRNAME, client_shaders_path, sizeof(client_shaders_path));
    if (size >= sizeof(client_shaders_path)) {
        fprintf(stderr, "vertex shader path too long (max %zu): %s\n", sizeof(client_vshader_path), client_vshader_path);
        return -1;
    }
    if (MKDIR(client_shaders_path) != 0 && errno != EEXIST) {
        fprintf(stderr, "cannot create directory \"%s\": mkdir failed\n", client_shaders_path);
        return -1;
    }

    // Create global environment paths for client
    size = cwk_path_join(client_shaders_path, CLIENT_VSHADER_FILENAME, client_vshader_path, sizeof(client_vshader_path));
    if (size >= sizeof(client_vshader_path)) {
        fprintf(stderr, "shaders path too long (max %zu): %s\n", sizeof(client_vshader_path), client_vshader_path);
        return -1;
    }

    size = cwk_path_join(client_shaders_path, CLIENT_FSHADER_FILENAME, client_fshader_path, sizeof(client_fshader_path));
    if (size >= sizeof(client_fshader_path)) {
        fprintf(stderr, "fragment shader path too long (max %zu): %s\n", sizeof(client_fshader_path), client_fshader_path);
        return -1;
    }

    size = cwk_path_join(project_root_path, CLIENT_LOG_FILENAME, client_log_path, sizeof(client_log_path));
    if (size >= sizeof(client_log_path)) {
        fprintf(stderr, "client log path too long (max %zu): %s\n", sizeof(client_log_path), client_log_path);
        return -1;
    }

    size = cwk_path_join(project_root_path, CLIENT_SETTINGS_FILENAME, client_settings_path, sizeof(client_settings_path));
    if (size >= sizeof(client_settings_path)) {
        fprintf(stderr, "client settings path too long (max %zu): %s\n", sizeof(client_settings_path), client_settings_path);
        return -1;
    }

    // Create global environment paths for server
    size = cwk_path_join(project_root_path, SERVER_LOG_FILENAME, server_log_path, sizeof(server_log_path));
    if (size >= sizeof(server_log_path)) {
        fprintf(stderr, "server log path too long (max %zu): %s\n", sizeof(server_log_path), server_log_path);
        return -1;
    }

    return 0;
}

int init_subsystems()
{
    // Initialise client environment
    if ((client_log_index = logger_add_log(client_log_path)) < 0) {
        fprintf(stderr, "Could not create client log\n");
        goto cleanup_client_log;
    }
    else {
        logger_log_message(client_log_index, LOG_INFO, "Welcome To The Client Log Of\n%s", PROJECT_NAME_ASCII_ART);
    }

    if (parse_client_settings(client_settings_path) != 0) {
        logger_log_message(client_log_index, LOG_ERROR, "Could not initialise client settings");
        goto cleanup_client_settings;
    }
    if (init_window() != 0) {
        logger_log_message(client_log_index, LOG_ERROR, "Could not initialise window");
        goto cleanup_client_window;
    }
    if (init_renderer(client_vshader_path, client_fshader_path) != 0) {
        logger_log_message(client_log_index, LOG_ERROR, "Could not initialise renderer");
        goto cleanup_client_renderer;
    }

    // Initialise server environment
    if ((server_log_index = logger_add_log(server_log_path)) < 0) {
        fprintf(stderr, "Could not create server log\n");
        goto cleanup_server_log;
    }
    else {
        logger_log_message(server_log_index, LOG_INFO, "Welcome To The Server Log Of\n%s", PROJECT_NAME_ASCII_ART);
    }

    return 0;

cleanup_server_log:
    terminate_renderer();
cleanup_client_renderer:
    terminate_window();
cleanup_client_window:
    terminate_settings();
cleanup_client_settings:
    terminate_logger();
cleanup_client_log:
    return -1;
}

void terminate_subsystems()
{
    terminate_renderer();
    terminate_window();
    terminate_settings();
    terminate_logger();
}
