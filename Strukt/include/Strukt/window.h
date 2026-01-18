#pragma once
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include <glad/glad.h>

struct strukt_window {
    GLFWwindow* window_handle;
    int window_width;
    int window_height;
    char* window_title;
};

int windowInit(struct strukt_window* window);
int windowShouldClose(struct strukt_window* window);
void windowSwapBuffers(struct strukt_window* window);
void windowPollEvents(struct strukt_window* window);
void windowTerminate(struct strukt_window* window);
