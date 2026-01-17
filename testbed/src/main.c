#include <Strukt/window.h>
#include <Strukt/logger.h>

int main()
{
    log_info("Hello World!");
    windowInit(500, 500, "Hello World");

    while(!windowShouldClose())
    {
        windowPollEvents();
        windowFlush();
    }

    windowTerminate();
    return 0;
}
