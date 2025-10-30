/*
 * Window.c is responsible for window creation, window handling and user input
*/
#pragma once
#include <GLFW/glfw3.h>
#include <settings.h>

int init_window(GLFWwindow** window, const char* title, int initial_width, int initial_height, bool is_fullscreen);
void terminate_window(GLFWwindow* window);
