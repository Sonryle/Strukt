#include "graphics.h"

#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

struct RendererOptions renderer_options = {
        640,
        480,
        "Strukt",
};
GLFWwindow* window;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int initRenderer(void)
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
                renderer_options.initial_width,
                renderer_options.initial_height,
                renderer_options.window_title,
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

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
                fprintf(stderr, "Failed to initialize GLAD\n");
                glfwTerminate();
                return -1;
        }
        
        glClearColor(0.47f, 0.35f, 0.8f, 1.0f);
        glViewport(0, 0, renderer_options.initial_width, renderer_options.initial_height);
        return 0;
}

int render()
{
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();

        return 0;
}

void terminateRenderer()
{
        glfwTerminate();
        return;
}

void framebuffer_size_callback(GLFWwindow* window, GLint width, GLint height)
{
        // glViewport(0, 0, width, height);
}
