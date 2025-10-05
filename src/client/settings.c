#include <string.h>
#include <stdio.h>

#include <tomlc17.h>

#include <file_operations.h>
#include <client/settings.h>
#include <client/logging.h>

void parse_window_settings();
void parse_renderer_settings();

static struct WindowSettings window_settings;
static struct RendererSettings renderer_settings;
struct Settings settings = {
        .window = &window_settings,
        .renderer = &renderer_settings,
};

toml_result_t toml_output;

int parse_client_settings()
{
        if (!file_exists(CLIENT_SETTINGS_FILENAME))
            // Copy 
            ;

        toml_output = toml_parse_file_ex(CLIENT_SETTINGS_FILENAME);
        if (!toml_output.ok)
        {
                log_err("unable to load \"%s\", Toml error \"%s\"", CLIENT_SETTINGS_FILENAME, toml_output.errmsg);
                return -1;
        }

        parse_window_settings();
        parse_renderer_settings();

        toml_free(toml_output);

        return 0;
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
        else log_warn("missing or invalid window.title property in %s", CLIENT_SETTINGS_FILENAME);

        if (initial_width.type == TOML_INT64) settings.window->initial_width = (int)initial_width.u.int64;
        else log_warn("missing or invalid window.initial_width property in %s", CLIENT_SETTINGS_FILENAME);

        if (initial_height.type == TOML_INT64) settings.window->initial_height = (int)initial_height.u.int64;
        else log_warn("missing or invalid window.initial_width property in %s", CLIENT_SETTINGS_FILENAME);

        if (fullscreen.type == TOML_BOOLEAN) settings.window->fullscreen = fullscreen.u.boolean;
        else log_warn("missing or invalid window.fullscreen property in %s", CLIENT_SETTINGS_FILENAME);
}

void parse_renderer_settings()
{
        /* Extract values */
        toml_datum_t bgr = toml_seek(toml_output.toptab, "renderer.bgr");
        toml_datum_t bgg = toml_seek(toml_output.toptab, "renderer.bgg");
        toml_datum_t bgb = toml_seek(toml_output.toptab, "renderer.bgb");

        /* Copy values into settings struct */
        if (bgr.type == TOML_FP64) settings.renderer->bgr = (float)bgr.u.fp64;
        else log_warn("settings.c - missing or invalid renderer.bgr property in %s", CLIENT_SETTINGS_FILENAME);

        if (bgg.type == TOML_FP64) settings.renderer->bgg = (float)bgg.u.fp64;
        else log_warn("settings.c - missing or invalid renderer.bgg property in %s", CLIENT_SETTINGS_FILENAME);

        if (bgb.type == TOML_FP64) settings.renderer->bgb = (float)bgb.u.fp64;
        else log_warn("settings.c - missing or invalid renderer.bgb property in %s", CLIENT_SETTINGS_FILENAME);
}
