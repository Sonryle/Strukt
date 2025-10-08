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

        while(!glfwWindowShouldClose(window))
        {
                temporary_render();

                /* Poll for and process events */
                glfwPollEvents();
        }

        terminate_client();
        return 0;
}

int setup_client_environment()
{
        fetch_project_details();
        if (init_client_log(project_data_dir))
        {
                fprintf(stderr, "main.c - init_clientlog() returned error\n");
                return -1;
        }
        parse_client_settings(project_data_dir);
        if (init_window())
                return -1;
        if (init_renderer(project_data_dir))
                return -1;

        log_info("Client Environment Successfully Initiated; Welcome To Strukt.");
        return 0;
}

void terminate_client()
{
        terminate_project_details();
        terminate_window();
        terminate_renderer();
        terminate_client_log();
}
