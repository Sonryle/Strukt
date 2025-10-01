/*
 * Window.c is responsible for window creation, window handling and user input
*/
#pragma once

#include <GLFW/glfw3.h>

struct WindowSettings {
        int initial_width;
        int initial_height;
        const char* window_title;
};

extern struct WindowSettings window_settings;
extern GLFWwindow* window;

int initWindow();
void terminateWindow();
