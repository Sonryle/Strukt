#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <tomlc17.h>

#include <main.h>
#include <ASCIIart.h>
#include <logger.h>
#include <settings.h>

#define EXTRACT_INT 5

void get_default_client_settings(struct ClientSettings* settings);
void get_default_server_settings(struct ServerSettings* settings);
void generate_client_settings_file(struct ClientSettings* settings, char* settings_path);
void generate_server_settings_file(struct ServerSettings* settings, char* settings_path);
void extract_client_settings(struct ClientSettings* settings, toml_result_t* toml_output);
void extract_server_settings(struct ServerSettings* settings, toml_result_t* toml_output);

int init_settings(struct ClientSettings* client_settings, struct ServerSettings* server_settings,
                const char* client_settings_path, const char* server_settings_path)
{
    /* 
    * fill client_settings & server_settings structs with default settings initially.
    * Default settings are overwritten by toml parser. If a setting is missing/invalid
    * then it wont be overwritten and the default value will be there in its place.
    */
    get_default_client_settings(client_settings);
    get_default_server_settings(server_settings);

    /* Check if setting files exist. If not, generate them */
    FILE* client_settings_exist = fopen(client_settings_path, "r");
    FILE* server_settings_exist = fopen(server_settings_path, "r");
    if (client_settings_exist == NULL) generate_client_settings_file(client_settings, client_settings_path);
    else fclose(client_settings_exist);
    if (server_settings_exist == NULL) generate_server_settings_file(server_settings, server_settings_path);
    else fclose(server_settings_exist);

    /* Parse settings file into toml_result_t struct */
    toml_result_t client_toml_output = toml_parse_file_ex(client_settings_path);
    toml_result_t server_toml_output = toml_parse_file_ex(server_settings_path);
    if (!client_toml_output.ok) {
        fprintf(stderr, "unable to load \"%s\", Toml error \"%s\"\n",
            client_settings_path, client_toml_output.errmsg);
        return -1;
    }
    if (!server_toml_output.ok) {
        fprintf(stderr, "unable to load \"%s\", Toml error \"%s\"\n",
            server_settings_path, server_toml_output.errmsg);
        return -1;
    }

    /* Extract all settings values from toml_result_t struct into settings structs*/
    extract_client_settings(client_settings, &client_toml_output);
    extract_server_settings(server_settings, &server_toml_output);

    /* Cleanup */
    toml_free(client_toml_output);
    toml_free(server_toml_output);
    return 0;
}

void get_default_client_settings(struct ClientSettings* settings)
{
    *settings = (struct ClientSettings){
    .window = {
        .title = "Welcome To Strukt",
        .initial_width = 500,
        .initial_height = 250,
        .fullscreen = false,
    },
    .renderer = {
        .bgr = 0.52f,
        .bgg = 0.25f,
        .bgb = 0.75f,
    },
    .log_level = LOG_INFO,
    };
}

void get_default_server_settings(struct ServerSettings* settings)
{
    *settings = (struct ServerSettings){
        .log_level = LOG_INFO,
    };
}

void generate_client_settings_file(struct ClientSettings* settings, char* settings_path)
{
    FILE* settings_file = fopen(settings_path, "w");
    if (settings_file == NULL) {
            logger_log_message(CLIENT_LOG, LOG_ERROR, "unable to create settings file (%s). Using default settings", settings_path);
            return;
    }

    /* Fun Welcome Banner */
    fprintf(settings_file, "# Welcome to the Client Settings of:\n%s\n\n", PROJECT_NAME_ASCII_ART5_COMMENTED_OUT);
    /* Logger settings */
    fprintf(settings_file, "[logger]\n");
    fprintf(settings_file, "log_level = \"%s\"\t# Options: DEBUG, INFO, WARNING, ERROR\n", log_level_to_string(settings->log_level));
    fprintf(settings_file, "\n");

    /* Window settings */
    fprintf(settings_file, "[window]\n");
    fprintf(settings_file, "title = \"%s\"\n", settings->window.title);
    fprintf(settings_file, "windowed_width = %d\t# Width in pixels when not in fullscreen\n", settings->window.initial_width);
    fprintf(settings_file, "windowed_height = %d\t# Height in pixels when not in fullscreen\n", settings->window.initial_height);
    fprintf(settings_file, "fullscreen = %s\n", (settings->window.fullscreen)? "true" : "false");
    fprintf(settings_file, "\n");

    /* Renderer settings */
    fprintf(settings_file, "[renderer]\n");
    fprintf(settings_file, "bgr = %.2f\n", settings->renderer.bgr);
    fprintf(settings_file, "bgg = %.2f\n", settings->renderer.bgg);
    fprintf(settings_file, "bgb = %.2f\n", settings->renderer.bgb);
    fprintf(settings_file, "\n");

    fclose(settings_file);
}

void generate_server_settings_file(struct ServerSettings* settings, char* settings_path)
{
    FILE* settings_file = fopen(settings_path, "w");
    if (settings_file == NULL) {
        logger_log_message(SERVER_LOG, LOG_ERROR, "unable to create settings file (%s). Using default settings", settings_path);
        return;
    }

    /* Logger settings */
    fprintf(settings_file, "log_level = %d\n", settings->log_level);
    fprintf(settings_file, "\n");

    fclose(settings_file);
}

void extract_client_settings(struct ClientSettings* settings, toml_result_t* toml_output)
{
    /* Logger Settings */
    toml_datum_t log_level = toml_seek(toml_output->toptab, "logger.log_level");
    if (strcmp(log_level.u.s, log_level_to_string(LOG_DEBUG)) == 0)
        settings->log_level = LOG_DEBUG;
    else if (strcmp(log_level.u.s, log_level_to_string(LOG_INFO)) == 0)
        settings->log_level = LOG_INFO;
    else if (strcmp(log_level.u.s, log_level_to_string(LOG_WARNING)) == 0)
        settings->log_level = LOG_WARNING;
    else if (strcmp(log_level.u.s, log_level_to_string(LOG_ERROR)) == 0)
        settings->log_level = LOG_ERROR;
    else
        logger_log_message(CLIENT_LOG, LOG_WARNING, "missing or invalid logger.log_level property in client settings. Using default value (%d)", settings->log_level);
    /* Window Settings */
    toml_datum_t title = toml_seek(toml_output->toptab, "window.title");
    if (title.type == TOML_STRING) strncpy(settings->window.title, title.u.s, CLIENT_MAX_WINDOW_TITLE_LEN);
    else logger_log_message(CLIENT_LOG, LOG_WARNING, "missing or invalid window.title property in client settings. Using default value (%s)", settings->window.title);

    toml_datum_t initial_width = toml_seek(toml_output->toptab, "window.windowed_width");
    if (initial_width.type == TOML_INT64) settings->window.initial_width = (int)initial_width.u.int64;
    else logger_log_message(CLIENT_LOG, LOG_WARNING, "missing or invalid window.windowed_width property in client settings. Using default value (%d)", settings->window.initial_width);

    toml_datum_t initial_height = toml_seek(toml_output->toptab, "window.windowed_height");
    if (initial_height.type == TOML_INT64) settings->window.initial_height = (int)initial_height.u.int64;
    else logger_log_message(CLIENT_LOG, LOG_WARNING, "missing or invalid window.windowed_height property in client settings. Using default value (%d)", settings->window.initial_height);

    toml_datum_t fullscreen = toml_seek(toml_output->toptab, "window.fullscreen");
    if (fullscreen.type == TOML_BOOLEAN) settings->window.fullscreen = fullscreen.u.boolean;
    else logger_log_message(CLIENT_LOG, LOG_WARNING, "missing or invalid window.fullscreen property in client settings. Using default value (%d)", settings->window.fullscreen);
    settings->window.fullscreen = false;
    /* Renderer Settings */
    toml_datum_t bgr = toml_seek(toml_output->toptab, "renderer.bgr");
    if (bgr.type == TOML_FP64) settings->renderer.bgr = (float)bgr.u.fp64;
    else logger_log_message(CLIENT_LOG, LOG_WARNING, "missing or invalid renderer.bgr property in client settings. Using default value (%f)", settings->renderer.bgr);

    toml_datum_t bgg = toml_seek(toml_output->toptab, "renderer.bgg");
    if (bgg.type == TOML_FP64) settings->renderer.bgg = (float)bgg.u.fp64;
    else logger_log_message(CLIENT_LOG, LOG_WARNING, "missing or invalid renderer.bgg property in client settings. Using default value (%f)", settings->renderer.bgg);

    toml_datum_t bgb = toml_seek(toml_output->toptab, "renderer.bgb");
    if (bgb.type == TOML_FP64) settings->renderer.bgb = (float)bgb.u.fp64;
    else logger_log_message(CLIENT_LOG, LOG_WARNING, "missing or invalid renderer.bgb property in client settings. Using default value (%f)", settings->renderer.bgb);
}

void extract_server_settings(struct ServerSettings* settings, toml_result_t* toml_output)
{
    /* Logger Settings */
    toml_datum_t log_level = toml_seek(toml_output->toptab, "logger.log_level");
    if (log_level.type == TOML_INT64) settings->log_level = (int)log_level.u.int64;
    else logger_log_message(SERVER_LOG, LOG_WARNING, "missing or invalid logger.log_level property in client settings. Using default value (%d)", settings->log_level);
}

void terminate_settings()
{
    return;
}