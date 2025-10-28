#include <stdlib.h>
#include <errno.h>

#include <cwalk.h>

#include <ASCIIart.h>
#include <main.h>
#include <logger.h>
#include <settings.h>
#include <client/window.h>
#include <client/renderer.h>

int init_app_context(struct AppContext* ctx);
int join_path(const char* path_name, const char* path1, const char* path2, const char* dest, int dest_size);
int setup_paths(struct AppPaths* paths);
int init_subsystems(struct AppContext* ctx);
void terminate_app_context(struct AppContext* ctx);

/* Default settings will be replaced by settings in settings file */
struct Settings settings = {
    .client_window = {
        .title = "Welcome To Strukt",
        .initial_width = 500,
        .initial_height = 250,
        .fullscreen = false,
    },
    .client_renderer = {
        .bgr = 0.52f,
        .bgg = 0.25f,
        .bgb = 0.75f,
    },
};

int main()
{
    struct AppContext app_context = { 0 };

    if (init_app_context(&app_context) != 0) {
        fprintf(stderr, "could not initiate app context\n");
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
    // Initialies global environment
    if (init_logger(ctx->paths.client_log_path, ctx->paths.server_log_path) != 0) {
        fprintf(stderr, "Could not initialise logger\n");
        goto cleanup_logger;
    }
    else {
        logger_log_message(CLIENT_LOG, LOG_INFO, "Welcome To The Client Log Of\n%s", PROJECT_NAME_ASCII_ART5);
    }

    // Initialise client environment
    if (parse_client_settings(&settings, ctx->paths.client_settings_path) != 0) {
        logger_log_message(CLIENT_LOG, LOG_ERROR, "Could not initialise client settings");
        goto cleanup_client_settings;
    }
    if (init_window(&ctx->graphics.window, &settings.client_window) != 0) {
        logger_log_message(CLIENT_LOG, LOG_ERROR, "Could not initialise window");
        goto cleanup_client_window;
    }
    if (init_renderer(&ctx->graphics.VBO, &ctx->graphics.VAO, &ctx->graphics.shader_program, ctx->paths.client_vshader_path, ctx->paths.client_fshader_path, &settings.client_renderer) != 0) {
        logger_log_message(CLIENT_LOG, LOG_ERROR, "Could not initialise renderer");
        goto cleanup_client_renderer;
    }
    // Initialise server environment
    // Nothing to do here yet

    return 0;

    terminate_renderer(&ctx->graphics.shader_program);
cleanup_client_renderer:
    terminate_window(ctx->graphics.window);
cleanup_client_window:
    terminate_settings();
cleanup_client_settings:
    terminate_logger();
cleanup_logger:
    return -1;
}

void terminate_app_context(struct AppContext* ctx)
{
    terminate_renderer(&ctx->graphics.shader_program);
    terminate_window(ctx->graphics.window);
    terminate_settings();
    terminate_logger();
}
