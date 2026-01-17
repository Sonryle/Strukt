#pragma once
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include <glad/glad.h>

struct struct_window {
    GLFWwindow* window_handle;
};

int windowInit(int window_width, int window_height, char* window_title);
int windowShouldClose();
void windowFlush();
void windowPollEvents();
void windowTerminate();
