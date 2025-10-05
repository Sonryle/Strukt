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

        log_debug("debug log test");
        log_info("info log test");
        log_err("error log test");
        log_warn("warning log test");

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
        if (parse_client_settings())
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
        terminate_renderer();
        terminate_client_log();
}
