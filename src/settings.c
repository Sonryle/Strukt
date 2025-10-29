#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <tomlc17.h>

#include <main.h>
#include <logger.h>
#include <settings.h>
#include <client/client.h>

void create_client_settings_file(struct Settings* settings, char* settings_dir);
void parse_client_window_settings(struct ClientWindowSettings* settings);
void parse_renderer_settings(struct ClientRendererSettings* settings);

toml_result_t toml_output;

int parse_client_settings(struct Settings* settings, char* settings_path)
{
    /*
    * I want to clean up the code just generally, but specifically this settings code.
    * I dont like how "parse_client_settings" will create a settings file for you if
    * one isnt already there. Thats not what I signed up for when I called the "parse" function.
    * I want that code to be brought upstream somewhere and for this code to just give an error
    * if no file is found.
    * 
    * Speaking of errors, the logging system only has two logs for the client and server. Settings
    * has nowhere to log to. I want to fix this (maybe by adding a "global environment" log..
    * Seems a bit finicky and I dont like it too much.
    */
    if (fopen(settings_path, "r") == NULL) {
        get_default_settings(&settings);
        create_client_settings_file(settings, settings_path);
    }

    toml_output = toml_parse_file_ex(settings_path);
    if (!toml_output.ok)
    {
            log_err("unable to load \"%s\", Toml error \"%s\"", settings_path, toml_output.errmsg);
            return -1;
    }

    parse_client_window_settings(&settings->client_window);
    parse_renderer_settings(&settings->client_renderer);

    toml_free(toml_output);
    return 0;
}

void get_default_settings(struct Settings* settings)
{
    *settings = (struct Settings) {
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
}

void create_client_settings_file(struct Settings* settings, char* settings_path)
{
        FILE* settings_file = fopen(settings_path, "w");
        if (settings_file == NULL) {
                log_err("unable to create default settings file");
                return;
        }

        /* Add window settings to file */
        fprintf(settings_file, "[window]\n");
        fprintf(settings_file, "title = \"%s\"\n", settings->client_window.title);
        fprintf(settings_file, "initial_width = %d\n", settings->client_window.initial_width);
        fprintf(settings_file, "initial_height = %d\n", settings->client_window.initial_height);
        fprintf(settings_file, "fullscreen = %s\n", (settings->client_window.fullscreen)? "true" : "false");
        fprintf(settings_file, "\n");

        /* Add renderer settings to file */
        fprintf(settings_file, "[renderer]\n");
        fprintf(settings_file, "bgr = %.2f\n", settings->client_renderer.bgr);
        fprintf(settings_file, "bgg = %.2f\n", settings->client_renderer.bgg);
        fprintf(settings_file, "bgb = %.2f\n", settings->client_renderer.bgb);

        fclose(settings_file);
}

void parse_client_window_settings(struct ClientWindowSettings* settings)
{
        /* Extract values */
        toml_datum_t title = toml_seek(toml_output.toptab, "window.title");
        toml_datum_t initial_width = toml_seek(toml_output.toptab, "window.initial_width");
        toml_datum_t initial_height = toml_seek(toml_output.toptab, "window.initial_height");
        toml_datum_t fullscreen = toml_seek(toml_output.toptab, "window.fullscreen");

        /* Copy values into settings struct */
        if (title.type == TOML_STRING) settings->title = strdup(title.u.s);
        else log_warn("missing or invalid window.title property in client settings");

        if (initial_width.type == TOML_INT64) settings->initial_width = (int)initial_width.u.int64;
        else log_warn("missing or invalid window.title property in client settings");

        if (initial_height.type == TOML_INT64) settings->initial_height = (int)initial_height.u.int64;
        else log_warn("missing or invalid window.title property in client settings");

        if (fullscreen.type == TOML_BOOLEAN) settings->fullscreen = fullscreen.u.boolean;
        else log_warn("missing or invalid window.title property in client settings");
}

void parse_renderer_settings(struct ClientRendererSettings* settings)
{
        /* Extract values */
        toml_datum_t bgr = toml_seek(toml_output.toptab, "renderer.bgr");
        toml_datum_t bgg = toml_seek(toml_output.toptab, "renderer.bgg");
        toml_datum_t bgb = toml_seek(toml_output.toptab, "renderer.bgb");

        /* Copy values into settings struct */
        if (bgr.type == TOML_FP64) settings->bgr = (float)bgr.u.fp64;
        else log_warn("missing or invalid window.title property in client settings");

        if (bgg.type == TOML_FP64) settings->bgg = (float)bgg.u.fp64;
        else log_warn("missing or invalid window.title property in client settings");

        if (bgb.type == TOML_FP64) settings->bgb = (float)bgb.u.fp64;
        else log_warn("missing or invalid window.title property in client settings");
}

void terminate_settings()
{
    return;
}