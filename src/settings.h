#pragma once
#include <tomlc17.h>

extern toml_result_t toml_output;

struct ClientWindowSettings {
        const char* title;
        int initial_width;
        int initial_height;
        bool fullscreen;
};

struct ClientRendererSettings {
        float bgr;
        float bgg;
        float bgb;
};

struct Settings {
        struct ClientWindowSettings client_window;
        struct ClientRendererSettings client_renderer;
};

int parse_client_settings(struct Settings* settings, char* settings_path);
void terminate_settings();