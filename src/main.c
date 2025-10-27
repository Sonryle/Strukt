#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include <GLFW/glfw3.h>
#include <cwalk.h>

#if defined (_WIN32)
#include <direct.h>
#define mkdir(path, mode) _mkdir(path)
#else
#include <sys/stat.h>
#endif

#include <client/logging.h>
#include <client/settings.h>
#include <client/window.h>
#include <client/renderer.h>

#define PROJECT_NAME "Strukt"
#define CLIENT_LOG_FILENAME "client_log.txt"
#define CLIENT_SETTINGS_FILENAME "client_settings.toml"
#define CLIENT_SHADERS_DIRNAME "shaders"
#define CLIENT_VSHADER_FILENAME "source.vs"
#define CLIENT_FSHADER_FILENAME "source.fs"

int setup_paths();
int init_subsystems();
void terminate_client();

static char project_root_path[FILENAME_MAX];

static char client_shaders_path[FILENAME_MAX];
static char client_vshader_path[FILENAME_MAX];
static char client_fshader_path[FILENAME_MAX];
static char client_log_path[FILENAME_MAX];
static char client_settings_path[FILENAME_MAX];

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

    terminate_client();
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
    if (mkdir(project_root_path, 0755) != 0 && errno != EEXIST) {
        fprintf(stderr, "cannot create directory \"%s\": mkdir failed\n", project_root_path);
        return -1;
    }

    size = cwk_path_join(project_root_path, CLIENT_SHADERS_DIRNAME, client_shaders_path, sizeof(client_shaders_path));
    if (size >= sizeof(client_shaders_path)) {
        fprintf(stderr, "vertex shader path too long (max %zu): %s\n", sizeof(client_vshader_path), client_vshader_path);
        return -1;
    }
    if (mkdir(client_shaders_path, 0755) != 0 && errno != EEXIST) {
        fprintf(stderr, "cannot create directory \"%s\": mkdir failed\n", client_shaders_path);
        return -1;
    }

    // Create global environment paths
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

    return 0;
}

int init_subsystems()
{
    // Initialise client environment
    if (init_logger(client_log_path) != 0) {
        fprintf(stderr, "Could not initialise logger\n");
        goto cleanup_logger;
    }
    if (parse_client_settings(client_settings_path) != 0) {
        log_err("Could not initialise client settings");
        goto cleanup_client_settings;
    }
    if (init_window() != 0) {
        log_err("Could not initialise window");
        goto cleanup_window;
    }
    if (init_renderer(client_vshader_path, client_fshader_path) != 0) {
        log_err("Could not initialise renderer");
        goto cleanup_renderer;
    }

    log_info("Client Environment Successfully Initiated; Welcome To Strukt.");
    return 0;

cleanup_renderer:
    terminate_window();
cleanup_window:
    terminate_settings();
cleanup_client_settings:
    terminate_logger();
cleanup_logger:
    return -1;
}

void terminate_client()
{
    terminate_renderer();
    terminate_window();
    terminate_settings();
    terminate_logger();
}
