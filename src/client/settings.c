#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <tomlc17.h>

#include <client/settings.h>
#include <client/logging.h>

void create_default_client_settings_file(char* settings_dir);
void parse_window_settings();
void parse_renderer_settings();

/* Default settings */
static struct WindowSettings window_settings = {
        .title = "Welcome To Strukt",
        .initial_width = 500,
        .initial_height = 250,
        .fullscreen = false,
};
static struct RendererSettings renderer_settings = {
        .bgr = 0.52f,
        .bgg = 0.25f,
        .bgb = 0.75f,
};
struct Settings settings = {
        .window = &window_settings,
        .renderer = &renderer_settings,
};

toml_result_t toml_output;

int parse_client_settings(char* settings_path)
{
        if (fopen(settings_path, "r") == NULL)
                create_default_client_settings_file(settings_path);

        toml_output = toml_parse_file_ex(settings_path);
        if (!toml_output.ok)
        {
                log_err("unable to load \"%s\", Toml error \"%s\"", settings_path, toml_output.errmsg);
                return -1;
        }

        parse_window_settings();
        parse_renderer_settings();

        toml_free(toml_output);
        return 0;
}

void create_default_client_settings_file(char* settings_path)
{
        FILE* settings_file = fopen(settings_path, "w");
        if (settings_file == NULL) {
                log_err("unable to create default settings file");
                return;
        }

        /* Add window settings to file */
        fprintf(settings_file, "[window]\n");
        fprintf(settings_file, "title = \"%s\"\n", settings.window->title);
        fprintf(settings_file, "initial_width = %d\n", settings.window->initial_width);
        fprintf(settings_file, "initial_height = %d\n", settings.window->initial_height);
        fprintf(settings_file, "fullscreen = %s\n", (settings.window->fullscreen)? "true" : "false");
        fprintf(settings_file, "\n");

        /* Add renderer settings to file */
        fprintf(settings_file, "[renderer]\n");
        fprintf(settings_file, "bgr = %.2f\n", settings.renderer->bgr);
        fprintf(settings_file, "bgg = %.2f\n", settings.renderer->bgg);
        fprintf(settings_file, "bgb = %.2f\n", settings.renderer->bgb);

        fclose(settings_file);
}

void parse_window_settings()
{
        /* Extract values */
        toml_datum_t title = toml_seek(toml_output.toptab, "window.title");
        toml_datum_t initial_width = toml_seek(toml_output.toptab, "window.initial_width");
        toml_datum_t initial_height = toml_seek(toml_output.toptab, "window.initial_height");
        toml_datum_t fullscreen = toml_seek(toml_output.toptab, "window.fullscreen");

        /* Copy values into settings struct */
        if (title.type == TOML_STRING) settings.window->title = strdup(title.u.s);
        else log_warn("missing or invalid window.title property in client settings");

        if (initial_width.type == TOML_INT64) settings.window->initial_width = (int)initial_width.u.int64;
        else log_warn("missing or invalid window.title property in client settings");

        if (initial_height.type == TOML_INT64) settings.window->initial_height = (int)initial_height.u.int64;
        else log_warn("missing or invalid window.title property in client settings");

        if (fullscreen.type == TOML_BOOLEAN) settings.window->fullscreen = fullscreen.u.boolean;
        else log_warn("missing or invalid window.title property in client settings");
}

void parse_renderer_settings()
{
        /* Extract values */
        toml_datum_t bgr = toml_seek(toml_output.toptab, "renderer.bgr");
        toml_datum_t bgg = toml_seek(toml_output.toptab, "renderer.bgg");
        toml_datum_t bgb = toml_seek(toml_output.toptab, "renderer.bgb");

        /* Copy values into settings struct */
        if (bgr.type == TOML_FP64) settings.renderer->bgr = (float)bgr.u.fp64;
        else log_warn("missing or invalid window.title property in client settings");

        if (bgg.type == TOML_FP64) settings.renderer->bgg = (float)bgg.u.fp64;
        else log_warn("missing or invalid window.title property in client settings");

        if (bgb.type == TOML_FP64) settings.renderer->bgb = (float)bgb.u.fp64;
        else log_warn("missing or invalid window.title property in client settings");
}

void terminate_settings()
{
    return;
}