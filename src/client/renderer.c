#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <client/logging.h>
#include <client/settings.h>
#include <client/renderer.h>
#include <client/window.h>

int init_renderer()
{
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
                // log_err("renderer.c - failed to initialize GLAD");
                glfwTerminate();
                return -1;
        }

        glClearColor(settings.renderer->bgr, settings.renderer->bgg, settings.renderer->bgb, 1.0f);
        glViewport(0, 0, settings.window->initial_width, settings.window->initial_height);

        return 0;
}

int render()
{
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);

        return 0;
}

void terminate_renderer()
{

}
