/*
    * I want to clean up the code just generally, but specifically this settings code.
    * I dont like how "parse_client_settings" will create a settings file for you if
    * one isnt already there. Thats not what I signed up for when I called the "parse" function.
    * I want that code to be brought upstream somewhere and for this code to just give an error
    * if no file is found.
    *
    * Speaking of errors, the logging system only has two logs for the client and server. Settings
    * has nowhere to log to. I want to fix this (maybe by adding a "global environment" log..
    * Seems a bit finicky and I dont like it too much.)
*/
#pragma once
#include <tomlc17.h>

#define CLIENT_MAX_WINDOW_TITLE_LEN 100

struct ClientWindowSettings {
    char title[CLIENT_MAX_WINDOW_TITLE_LEN];
    int initial_width;
    int initial_height;
    bool fullscreen;
};

struct ClientRendererSettings {
    float bgr;
    float bgg;
    float bgb;
};

struct ClientSettings {
    struct ClientWindowSettings window;
    struct ClientRendererSettings renderer;
    int log_level;
};

struct ServerSettings {
    int log_level;
};

int init_settings(struct ClientSettings* client_settings, struct ServerSettings* server_settings,
                const char* client_settings_path, const char* server_settings_path);
void terminate_settings();