#define DEBUG
#include <GLFW/glfw3.h>

#include <client/logging.h>
#include <client/settings.h>
#include <client/window.h>
#include <client/renderer.h>

int setup_client_environment();
void terminate_client();

int main()
{
        if (setup_client_environment())
                return -1;
        LOG_DEBUG("Hello this is a debug log");
        LOG_ERR("Hello this is an error log");
        LOG_INFO("Hello this is an info log");
        LOG_WARN("Hello this is a warning log");

        while(!glfwWindowShouldClose(window))
        {
                render();

                /* Poll for and process events */
                glfwPollEvents();
        }

        terminate_client();
        return 0;
}

int setup_client_environment()
{
        if (init_client_log())
        {
                fprintf(stderr, "main.c - init_clientlog() returned error\n");
                return -1;
        }
        if (init_settings())
                return -1;
        if (init_window())
                return -1;
        if (init_renderer())
                return -1;

        return 0;
}

void terminate_client()
{
        terminate_window();
        terminate_client_log();
}
