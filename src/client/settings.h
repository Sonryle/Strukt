/*
 * Settings.h is responsible for parsing and assigning the values
 * from client settings file into global settings structure.
*/
#pragma once
#define CLIENT_SETTINGS_FILENAME "client_settings.toml"
#include <tomlc17.h>

extern toml_result_t toml_output;

struct WindowSettings {
        const char* title;
        long initial_width;
        long initial_height;
        bool fullscreen;
};

struct RendererSettings {
        float bgr;
        float bgg;
        float bgb;
};

/* Settings struct cannot be a constant because settings are loaded during runtime */
extern struct Settings {
        struct WindowSettings* window;
        struct RendererSettings* renderer;
} settings;

int init_settings();
