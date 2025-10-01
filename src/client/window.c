#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <tomlc17.h>

#include "window.h"

struct WindowSettings window_settings;
GLFWwindow* window;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
int load_window_settings();

int initWindow(void)
{
        load_window_settings();

        if (!glfwInit())
        {
                fprintf(stderr, "Error - GLFW failed to initialize");
                return -1;
        }
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        window = glfwCreateWindow(
                window_settings.initial_width,
                window_settings.initial_height,
                window_settings.window_title,
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

int load_window_settings()
{
        toml_result_t result = toml_parse_file_ex("settings.toml");
        if (!result.ok)
        {
                fprintf(stderr, "Error - %s\n", result.errmsg);
        }

        // Extract Values
        toml_datum_t title = toml_seek(result.toptab, "window.title");
        toml_datum_t width = toml_seek(result.toptab, "window.width");
        toml_datum_t height = toml_seek(result.toptab, "window.height");

        if (title.type == TOML_STRING)
                window_settings.window_title = title.u.s;
        else if (title.type != TOML_UNKNOWN)
                fprintf(stderr, "Error - Invalid Toml Data for [window] title.\n");
        
        window_settings.initial_height = 100;
        window_settings.initial_width = 100;

        return 0;
}

void framebuffer_size_callback(GLFWwindow* window, GLint width, GLint height)
{
        // glViewport(0, 0, width, height);
}
