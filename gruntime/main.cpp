#include <GLFW/glfw3.h>
#include <cstdio>
#include "core/core.h"

int main(void)
{
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }

    GLFWwindow *window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    rpp::PrintHello();

    while (!glfwWindowShouldClose(window))
    {
        // glClear(GL_COLOR_BUFFER_BIT);

        // Render OpenGL here

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}