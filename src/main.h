#pragma once
#include <stdio.h>

#include <GLFW/glfw3.h>

#include <logger.h>

#if defined (_WIN32)
#include <direct.h>
#define MKDIR(path) _mkdir(path)
#else
#include <sys/stat.h>
#define MKDIR(path) mkdir(path, 0755)
#endif

#define PROJECT_NAME "Strukt"
#define PROJECT_NAME_ASCII_ART  " .----------------.  .----------------.  .----------------.  .----------------.  .----------------.  .----------------. \n"\
								"| .--------------. || .--------------. || .--------------. || .--------------. || .--------------. || .--------------. |\n"\
								"| |    _______   | || |  _________   | || |  _______     | || | _____  _____ | || |  ___  ____   | || |  _________   | |\n"\
								"| |   /  ___  |  | || | |  _   _  |  | || | |_   __ \\    | || ||_   _||_   _|| || | |_  ||_  _|  | || | |  _   _  |  | |\n"\
								"| |  |  (__ \\_|  | || | |_/ | | \\_|  | || |   | |__) |   | || |  | |    | |  | || |   | |_/ /    | || | |_/ | | \\_|  | |\n"\
								"| |   '.___`-.   | || |     | |      | || |   |  __ /    | || |  | '    ' |  | || |   |  __'.    | || |     | |      | |\n"\
								"| |  |`\\____) |  | || |    _| |_     | || |  _| |  \\ \\_  | || |   \\ `--' /   | || |  _| |  \\ \\_  | || |    _| |_     | |\n"\
								"| |  |_______.'  | || |   |_____|    | || | |____| |___| | || |    `.__.'    | || | |____||____| | || |   |_____|    | |\n"\
								"| |              | || |              | || |              | || |              | || |              | || |              | |\n"\
								"| '--------------' || '--------------' || '--------------' || '--------------' || '--------------' || '--------------' |\n"\
								"'----------------'  '----------------'  '----------------'  '----------------'  '----------------'  '----------------'"
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

struct AppLogs {
	int client_log_index;
	int server_log_index;
};

struct AppContext {
	struct AppPaths paths;
	struct Logger client_logger;
	struct Logger server_logger;
	GLFWwindow* window;
};

extern struct AppContext app_context;