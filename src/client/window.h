/*
 * Window.c is responsible for window creation, window handling and user input
*/
#pragma once
#include <GLFW/glfw3.h>

extern GLFWwindow* window;

int init_window();
void terminate_window();
