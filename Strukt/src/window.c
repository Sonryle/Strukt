#include <Strukt/window.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <Strukt/logger.h>

static GLFWwindow* window = NULL;

int windowInit(int window_width, int window_height, char* window_title)
{
    log_info("Initiating Graphics");
    if (window != NULL) {
        log_warn("Cannot initiate window more than once");
        return -1;
    }

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    window = glfwCreateWindow(window_width, window_height, window_title, NULL, NULL);
    if (window == NULL) {
        log_fatal("Failed to create GLFW window");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == 0) {
        log_fatal("Failed to initialise GLAD openGL function loader");
        glfwTerminate();
        return -1;
    }
    
    return 0;
}

int windowShouldClose()
{
    return glfwWindowShouldClose(window);
}

void windowFlush()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);
}

void windowPollEvents()
{
    glfwPollEvents();
}

void windowTerminate()
{
    if (window != NULL) {
        glfwDestroyWindow(window);
        glfwTerminate();
    }
    else
        log_warn("terminating window while window == NULL");
}
