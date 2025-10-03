#define DEBUG
#include <GLFW/glfw3.h>

#include "client/logging.h"
#include "client/settings.h"
#include "client/window.h"
#include "client/renderer.h"

int setup_client_environment();
void terminate_client();

int main()
{
        if (setup_client_environment())
                return -1;
        log_debug("Hello this is a debug log");
        log_err("Hello this is an error log");
        log_info("Hello this is an info log");
        log_warn("Hello this is a warning log");

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
        {
                // log_err("main.c - init_settings() returned error");
                return -1;
        }
        if (init_window())
        {
                // log_err("main.c - init_window() returned error");
                return -1;
        }
        if (init_renderer())
        {
                // log_err("main.c - init_renderer() returned error");
                return -1;
        }

        return 0;
}

void terminate_client()
{
        terminate_window();
        terminate_client_log();
}
