#include <Strukt/window.h>
#include <Strukt/renderer.h>
#include <Strukt/logger.h>

struct struct_window window = {
    .window_handle = NULL,
};

int windowInit(int window_width, int window_height, char* window_title)
{
    log_info("Initiating Window");
    if (window.window_handle != NULL) {
        log_warn("Cannot initiate window more than once");
        return -1;
    }

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    window.window_handle = glfwCreateWindow(window_width, window_height, window_title, NULL, NULL);
    if (window.window_handle == NULL) {
        log_fatal("Failed to create GLFW window");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window.window_handle);

    if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == 0) {
        log_fatal("Failed to initialise GLAD openGL function loader");
        glfwTerminate();
        return -1;
    }
    
    return 0;
}

int windowShouldClose()
{
    return glfwWindowShouldClose(window.window_handle);
}

void windowFlush()
{
    rendererFlush();
    glfwSwapBuffers(window.window_handle);
}

void windowPollEvents()
{
    glfwPollEvents();
}

void windowTerminate()
{
    if (window.window_handle != NULL) {
        glfwDestroyWindow(window.window_handle);
        glfwTerminate();
    }
    else
        log_warn("terminating window while window == NULL");
}
