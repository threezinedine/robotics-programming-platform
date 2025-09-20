#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include "core/core.h"

int main(void)
{
    rpp::print("Starting application...\n");
    rpp::print("Starting application...\n", rpp::ConsoleColor::RED);
    rpp::print("Starting application...\n", rpp::ConsoleColor::GREEN);
    rpp::print("Starting application...\n", rpp::ConsoleColor::BLUE);
    rpp::print("Starting application...\n", rpp::ConsoleColor::YELLOW);

    if (!glfwInit())
    {
        rpp::print("Failed to initialize GLFW\n");
        return -1;
    }

    GLFWwindow *window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        rpp::print("Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    rpp::PrintHello();

    if (glewInit() != GLEW_OK)
    {
        rpp::print("Failed to initialize GLEW\n");
        glfwTerminate();
        return -1;
    }

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

        // Render OpenGL here

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}