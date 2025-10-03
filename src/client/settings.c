#include <string.h>

#include "settings.h"
#include "logging.h"
#include "tomlc17.h"

void init_window_settings();
void init_renderer_settings();

toml_result_t toml_output;
static struct WindowSettings window_settings;
static struct RendererSettings renderer_settings;
struct Settings settings = {
        .window = &window_settings,
        .renderer = &renderer_settings,
};

int init_settings()
{
        toml_output = toml_parse_file_ex(STRUKT_CLIENT_SETTINGS_FILENAME);
        if (!toml_output.ok)
        {
                // log_err("settings.c unable to load %s", STRUKT_CLIENT_SETTINGS_FILENAME);
                return -1;
        }

        settings.window->title = "work you stupid clanker";
        init_window_settings();
        init_renderer_settings();

        toml_free(toml_output);

        return 0;
}

void init_window_settings()
{
        /* Extract values */
        toml_datum_t title = toml_seek(toml_output.toptab, "window.title");
        toml_datum_t initial_width = toml_seek(toml_output.toptab, "window.initial_width");
        toml_datum_t initial_height = toml_seek(toml_output.toptab, "window.initial_height");
        toml_datum_t fullscreen = toml_seek(toml_output.toptab, "window.fullscreen");

        /* Copy values into settings struct */
        if (title.type == TOML_STRING) settings.window->title = strdup(title.u.s);
        // else log_warn("settings.c - missing or invalid window.title property in %s", STRUKT_CLIENT_SETTINGS_FILENAME);

        if (initial_width.type == TOML_INT64) settings.window->initial_width = initial_width.u.int64;
        // else log_warn("settings.c - missing or invalid window.initial_width property in %s", STRUKT_CLIENT_SETTINGS_FILENAME);

        if (initial_height.type == TOML_INT64) settings.window->initial_height = initial_height.u.int64;
        // else log_warn("settings.c - missing or invalid window.initial_width property in %s", STRUKT_CLIENT_SETTINGS_FILENAME);

        if (fullscreen.type == TOML_BOOLEAN) settings.window->fullscreen = fullscreen.u.boolean;
        // else log_warn("settings.c - missing or invalid window.fullscreen property in %s", STRUKT_CLIENT_SETTINGS_FILENAME);
}

void init_renderer_settings()
{
        /* Extract values */
        toml_datum_t bgr = toml_seek(toml_output.toptab, "renderer.bgr");
        toml_datum_t bgg = toml_seek(toml_output.toptab, "renderer.bgg");
        toml_datum_t bgb = toml_seek(toml_output.toptab, "renderer.bgb");

        /* Copy values into settings struct */
        if (bgr.type == TOML_FP64) settings.renderer->bgr = bgr.u.fp64;
        // else log_warn("settings.c - missing or invalid renderer.bgr property in %s", STRUKT_CLIENT_SETTINGS_FILENAME);

        if (bgg.type == TOML_FP64) settings.renderer->bgg = bgg.u.fp64;
        // else log_warn("settings.c - missing or invalid renderer.bgg property in %s", STRUKT_CLIENT_SETTINGS_FILENAME);

        if (bgb.type == TOML_FP64) settings.renderer->bgb = bgb.u.fp64;
        // else log_warn("settings.c - missing or invalid renderer.bgb property in %s", STRUKT_CLIENT_SETTINGS_FILENAME);
}
