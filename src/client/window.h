/*
 * Window.c is responsible for window creation, window handling and user input
*/
#pragma once

#include <GLFW/glfw3.h>

struct WindowOptions {
        const int initial_width;
        const int initial_height;
        const char* window_title;

};

extern struct WindowOptions window_options;
extern GLFWwindow* window;

int initWindow();
void terminateWindow();
