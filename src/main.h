#pragma once

#if defined (_WIN32)
#include <direct.h>
#else
#include <sys/stat.h>
#endif
#include <stdio.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <logger.h>
#include <settings.h>

#define PROJECT_NAME "Strukt"
#define CLIENT_LOG_FILENAME "client_log.txt"
#define CLIENT_SETTINGS_FILENAME "client_settings.toml"
#define CLIENT_SHADERS_DIRNAME "shaders"
#define CLIENT_VSHADER_FILENAME "source.vs"
#define CLIENT_FSHADER_FILENAME "source.fs"
#define SERVER_LOG_FILENAME "server_log.txt"

struct AppPaths {
	char project_root_path[FILENAME_MAX];
	char client_shaders_path[FILENAME_MAX];
	char client_vshader_path[FILENAME_MAX];
	char client_fshader_path[FILENAME_MAX];
	char client_log_path[FILENAME_MAX];
	char client_settings_path[FILENAME_MAX];
	char server_log_path[FILENAME_MAX];
};

struct AppGraphics {
	GLFWwindow* window;
	GLuint VBO;
	GLuint VAO;
	GLuint shader_program;
};

struct AppContext {
	struct AppPaths paths;
	struct AppGraphics graphics;
	struct Settings settings;
};