/*
 * Graphics.c is responsible for window creation, window handling and rendering.
*/
#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

struct RendererOptions {
        const int initial_width;
        const int initial_height;
        const char* window_title;

};

extern struct RendererOptions renderer_options;
extern GLFWwindow* window;

int initRenderer();
int flushRenderer();
int render();
void terminateRenderer();
