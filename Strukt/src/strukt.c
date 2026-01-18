#include <Strukt/strukt.h>
#include <Strukt/window.h>
#include <Strukt/renderer.h>

void struktReturnDefaultInstance(struct strukt_instance* instance)
{
    instance->window = (struct strukt_window) {
        .window_handle = NULL,
        .window_width = 500,
        .window_height = 500,
        .window_title = "Strukt | Untitled Project"
    };
}

void struktRun(struct strukt_instance* instance)
{
    windowInit(&instance->window);
    rendererInit();

    while(!windowShouldClose(&instance->window))
    {
        windowPollEvents(&instance->window);
        rendererFlush();
        windowSwapBuffers(&instance->window);
    }

    windowTerminate(&instance->window);
}
