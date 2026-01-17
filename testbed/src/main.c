#include "Strukt/renderer.h"
#include <Strukt/window.h>
#include <Strukt/logger.h>

int main()
{
    log_info("Hello World!");
    windowInit(500, 450, "Strukt | Beta");
    rendererInit();

    while(!windowShouldClose())
    {
        windowPollEvents();
        windowFlush();
    }

    windowTerminate();
    return 0;
}
