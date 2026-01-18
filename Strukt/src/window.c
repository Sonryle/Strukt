#include <Strukt/window.h>
#include <Strukt/renderer.h>
#include <Strukt/logger.h>

void __framebufferSizeCallback(GLFWwindow* window, int width, int height);



int windowInit(struct strukt_window* window)
{
    log_info("Initiating Window");
    if (window->window_handle != NULL) {
        log_warn("Cannot initiate window more than once");
        return -1;
    }

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    window->window_handle = glfwCreateWindow(window->window_width,
                                             window->window_height,
                                             window->window_title,
                                             NULL, NULL);
    if (window->window_handle == NULL) {
        log_fatal("Failed to create GLFW window");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window->window_handle);
    glfwSetFramebufferSizeCallback(window->window_handle, __framebufferSizeCallback);

    if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == 0) {
        log_fatal("Failed to initialise GLAD openGL function loader");
        glfwTerminate();
        return -1;
    }
    
    return 0;
}

int windowShouldClose(struct strukt_window* window)
{
    return glfwWindowShouldClose(window->window_handle);
}

void windowSwapBuffers(struct strukt_window* window)
{
    glfwSwapBuffers(window->window_handle);
}

void windowPollEvents(struct strukt_window* window)
{
    glfwPollEvents();
}

void windowTerminate(struct strukt_window* window)
{
    if (window->window_handle != NULL) {
        glfwDestroyWindow(window->window_handle);
        glfwTerminate();
    }
    else
        log_warn("terminating window while window == NULL");
}

void __framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
