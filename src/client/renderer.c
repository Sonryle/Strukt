#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "renderer.h"
#include "window.h"

int initRenderer()
{
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
                fprintf(stderr, "Failed to initialize GLAD\n");
                glfwTerminate();
                return -1;
        }
        
        glClearColor(0.47f, 0.35f, 0.8f, 1.0f);
        glViewport(0, 0, window_settings.initial_width, window_settings.initial_height);

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

}
