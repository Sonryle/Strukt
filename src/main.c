#include <stdio.h>
#include <GLFW/glfw3.h>

#include "client/window.h"
#include "client/renderer.h"

int main()
{
        printf("Welcome To Strukt\n");
        initWindow();
        initRenderer();

        while(!glfwWindowShouldClose(window))
        {
                render();
        }

        terminateWindow();
        return 0;
}
