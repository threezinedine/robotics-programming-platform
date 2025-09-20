#include "platforms/graphics/graphics.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

///< OpenGL is used for Windows, Linux and MacOS platforms.
#if defined(RPP_PLATFORM_WINDOWS) || defined(RPP_PLATFORM_LINUX) || defined(RPP_PLATFORM_MACOS)

namespace rpp
{
    Graphics::Graphics()
    {
    }

    Graphics::~Graphics()
    {
    }

    b8 Graphics::Init()
    {
        if (!glfwInit())
        {
            return FALSE;
        }

        return TRUE;
    }

    void Graphics::Shutdown()
    {
        glfwTerminate();
    }

    Scope<Window> Graphics::CreateWindow(u32 width, u32 height, const char *title)
    {
        auto window = CreateScope<Window>(width, height, title);

        if (glewInit() != GLEW_OK)
        {
            return nullptr;
        }

        return window;
    }

    Window::Window(u32 width, u32 height, const char *title)
        : m_width(width), m_height(height), m_title(title)
    {
        m_window = glfwCreateWindow(width, height, title, NULL, NULL);
        glfwMakeContextCurrent((GLFWwindow *)m_window);
    }

    Window::~Window()
    {
    }

    b8 Window::ShouldWindowClose()
    {
        return glfwWindowShouldClose((GLFWwindow *)m_window);
    }

    void Window::PollEvents()
    {
        glfwPollEvents();
    }

    b8 Window::ExecuteCommand(const GraphicsCommandData &command)
    {
        switch (command.type)
        {
        case GraphicsCommandType::CLEAR_COLOR:
        {
            ClearColorCommandData *color = (ClearColorCommandData *)command.data;
            glClear(GL_COLOR_BUFFER_BIT);
            glClearColor(color->color[0], color->color[1], color->color[2], color->color[3]);
            return TRUE;
        }
        case GraphicsCommandType::SWAP_BUFFERS:
        {
            glfwSwapBuffers((GLFWwindow *)m_window);
            return TRUE;
        }
        default:
            return FALSE;
        }
    }
} // namespace rpp

#endif