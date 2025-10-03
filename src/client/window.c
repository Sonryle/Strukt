#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <tomlc17.h>

#include <client/logging.h>
#include <client/settings.h>
#include <client/window.h>

GLFWwindow* window;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int init_window(void)
{
        if (!glfwInit())
        {
                // log_err("window.c - GLFW failed to initialize");
                return -1;
        }
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        window = glfwCreateWindow(
                settings.window->initial_width,
                settings.window->initial_height,
                settings.window->title,
                NULL,
                NULL
        );

        if (window == NULL)
        {
                // log_err("window.c - failed to create GLFW window");
                glfwTerminate();
                return -1;
        }

        glfwMakeContextCurrent(window);
        return 0;
}

void terminate_window()
{
        glfwTerminate();
}

void framebuffer_size_callback(GLFWwindow* window, GLint width, GLint height)
{
        glViewport(0, 0, width, height);
}
