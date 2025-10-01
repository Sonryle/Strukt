#include <stdio.h>
#include "graphics.h"

int main()
{
        printf("Welcome To Strukt\n");
        initRenderer();

        while(!glfwWindowShouldClose(window))
        {
                render();
        }

        terminateRenderer();
        return 0;
}
