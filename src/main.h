#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <logger.h>
#include <settings.h>

#define PROJECT_NAME "Strukt"
#define MAX_PATH_LENGTH 512

// Filenames
#define CLIENT_LOG_FILENAME "client_log.txt"
#define CLIENT_SETTINGS_FILENAME "client_settings.toml"
#define CLIENT_SHADERS_DIRNAME "shaders"
#define CLIENT_VSHADER_FILENAME "source.vs"
#define CLIENT_FSHADER_FILENAME "source.fs"
#define SERVER_LOG_FILENAME "server_log.txt"
#define SERVER_SETTINGS_FILENAME "server_settings.toml"

struct AppPaths {
	char project_root_path[MAX_PATH_LENGTH];
	char client_shaders_path[MAX_PATH_LENGTH];
	char client_vshader_path[MAX_PATH_LENGTH];
	char client_fshader_path[MAX_PATH_LENGTH];
	char client_log_path[MAX_PATH_LENGTH];
	char client_settings_path[MAX_PATH_LENGTH];
	char server_log_path[MAX_PATH_LENGTH];
	char server_settings_path[MAX_PATH_LENGTH];
};

struct ClientGraphics {
	GLFWwindow* window;
	GLuint VBO;
	GLuint VAO;
	GLuint shader_program;
};

struct ClientState {
	struct ClientSettings client_settings;
	struct ClientGraphics graphics;
};

struct ServerState {
	struct ServerSettings server_settings;
};

struct GlobalState {
	struct AppPaths paths;
	struct ClientState client_state;
	struct ServerState server_state;
};