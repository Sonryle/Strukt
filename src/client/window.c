#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "window.h"

struct WindowOptions window_options = {
        640,
        480,
        "Strukt",
};
GLFWwindow* window;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int initWindow(void)
{
        if (!glfwInit())
        {
                fprintf(stderr, "Error - GLFW failed to initialize");
                return -1;
        }
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        window = glfwCreateWindow(
                window_options.initial_width,
                window_options.initial_height,
                window_options.window_title,
                NULL,
                NULL
        );

        if (window == NULL)
        {
                fprintf(stderr, "Failed to create GLFW window");
                glfwTerminate();
                return -1;
        }

        glfwMakeContextCurrent(window);
        return 0;
}

void terminateWindow()
{
        glfwTerminate();
        return;
}

void framebuffer_size_callback(GLFWwindow* window, GLint width, GLint height)
{
        // glViewport(0, 0, width, height);
}
