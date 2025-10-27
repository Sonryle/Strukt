#pragma once

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

extern int client_log_index;
extern int server_log_index;