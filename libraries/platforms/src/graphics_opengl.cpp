#include "platforms/graphics/graphics.h"

#if defined(RPP_GRAPHICS_BACKEND_OPENGL)
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "platforms/console.h"
#include <cstdio>
#include <functional>

#if defined(RPP_DEBUG)
#define GL_ASSERT(expression)                                                                                                    \
    expression;                                                                                                                  \
    {                                                                                                                            \
        GLenum error = glGetError();                                                                                             \
        if (error != GL_NO_ERROR)                                                                                                \
        {                                                                                                                        \
            char errorMessage[2048];                                                                                             \
            char finalMessage[4096];                                                                                             \
            u32 size = sizeof(errorMessage);                                                                                     \
            getError(error, errorMessage, &size);                                                                                \
            std::snprintf(finalMessage, sizeof(finalMessage), "OpenGL Error: %s at: %s:%d\n", errorMessage, __FILE__, __LINE__); \
            print(finalMessage, ConsoleColor::RED);                                                                              \
            __debugbreak();                                                                                                      \
        }                                                                                                                        \
    }
#else
#define GL_ASSERT(expression) expression
#endif

namespace rpp
{
    static void getError(GLenum error, char *message, u32 *size)
    {
        switch (error)
        {
        case GL_NO_ERROR:
            std::snprintf(message, *size, "No error has been recorded.");
            break;
        case GL_INVALID_ENUM:
            std::snprintf(message, *size, "An unacceptable value is specified for an enumerated argument. The offending command is ignored and has no other side effect than to set the error flag.");
            break;
        case GL_INVALID_VALUE:
            std::snprintf(message, *size, "A numeric argument is out of range. The offending command is ignored and has no other side effect than to set the error flag.");
            break;
        case GL_INVALID_OPERATION:
            std::snprintf(message, *size, "The specified operation is not allowed in the"
                                          "current state. The offending command is ignored and has no other side effect than to set the error flag.");
            break;
        case GL_STACK_OVERFLOW:
            std::snprintf(message, *size, "This command would cause a stack overflow."
                                          " The offending command is ignored and has no other side effect than to set the error flag.");
            break;
        case GL_STACK_UNDERFLOW:
            std::snprintf(message, *size, "This command would cause a stack underflow."
                                          " The offending command is ignored and has no other side effect than to set the error flag.");
            break;
        case GL_OUT_OF_MEMORY:
            std::snprintf(message, *size, "There is not enough memory left to execute the command. The state of the GL is undefined, except for the state of the error flags, after this error is recorded.");
            break;
        default:
            std::snprintf(message, *size, "Unknown error.");
        };
    }

    b8 Graphics::Init()
    {
        glfwSetErrorCallback([](int error, const char *description)
                             {
            char errorMessage[256];
            std::snprintf(errorMessage, sizeof(errorMessage), "GLFW Error (code %d): %s\n", error, description);
            print(errorMessage, ConsoleColor::RED); });

        if (!glfwInit())
        {
            return FALSE;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

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

    static GLenum convertAtomicTypeToGLenum(AtomicType type)
    {
        switch (type)
        {
        case AtomicType::FLOAT:
            return GL_FLOAT;
        case AtomicType::INT:
            return GL_INT;
        case AtomicType::UINT:
            return GL_UNSIGNED_INT;
        case AtomicType::DOUBLE:
            return GL_DOUBLE;
        default:
            return GL_FLOAT; // Default to float if unknown
        }
    }

    static u32 atomicTypeSize(AtomicType type)
    {
        switch (type)
        {
        case AtomicType::FLOAT:
            return sizeof(float);
        case AtomicType::INT:
            return sizeof(int);
        case AtomicType::UINT:
            return sizeof(unsigned int);
        case AtomicType::DOUBLE:
            return sizeof(double);
        default:
            return sizeof(float); // Default to float if unknown
        }
    }

    b8 Window::ExecuteCommand(const GraphicsCommandData &command)
    {
        switch (command.type)
        {
        case GraphicsCommandType::CLEAR_COLOR:
        {
            ClearColorCommandData *color = (ClearColorCommandData *)command.data;
            GL_ASSERT(glClear(GL_COLOR_BUFFER_BIT));
            GL_ASSERT(glClearColor(color->color[0], color->color[1], color->color[2], color->color[3]));
            return TRUE;
        }
        case GraphicsCommandType::SWAP_BUFFERS:
        {
            glfwSwapBuffers((GLFWwindow *)m_window);
            return TRUE;
        }
        case GraphicsCommandType::CREATE_TEXTURE:
        {
            CreateTextureCommandData *textureData = (CreateTextureCommandData *)command.data;
            GL_ASSERT(glGenTextures(1, textureData->textureId));
            GL_ASSERT(glBindTexture(GL_TEXTURE_2D, *textureData->textureId));

            GL_ASSERT(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
            GL_ASSERT(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

            if (textureData->numberOfChannels == 3)
            {
                GL_ASSERT(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureData->width, textureData->height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData->pixelData));
            }
            else if (textureData->numberOfChannels == 4)
            {
                GL_ASSERT(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureData->width, textureData->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData->pixelData));
            }
            else
            {
                GL_ASSERT(glDeleteTextures(1, textureData->textureId));
                textureData->textureId = 0;
                return FALSE;
            }

            GL_ASSERT(glBindTexture(GL_TEXTURE_2D, 0)); // Unbind the texture

            return TRUE;
        }
        case GraphicsCommandType::DELETE_TEXTURE:
        {
            DeleteTextureCommandData *deleteData = (DeleteTextureCommandData *)command.data;
            glDeleteTextures(1, &deleteData->textureId);
            return TRUE;
        }
        case GraphicsCommandType::CREATE_VERTEX_BUFFER:
        {
            CreateVertexBufferCommandData *vbData = (CreateVertexBufferCommandData *)command.data;
            GL_ASSERT(glGenBuffers(1, vbData->bufferId));
            GL_ASSERT(glBindBuffer(GL_ARRAY_BUFFER, *vbData->bufferId));

            // Retrieve layout final result
            u32 vertexSize = 0;
            for (u8 i = 0; i < vbData->layoutCount; i++)
            {
                vertexSize += atomicTypeSize(vbData->layout[i].atomic) * vbData->layout[i].elementCount;
            }

            // TODO: Update other methods later. Now just support TRIANGLE type.
            GL_ASSERT(glBufferData(GL_ARRAY_BUFFER, vbData->count * vertexSize, vbData->data, GL_STATIC_DRAW));

            GL_ASSERT(glGenVertexArrays(1, vbData->arrayId));
            GL_ASSERT(glBindVertexArray(*vbData->arrayId));
            GL_ASSERT(glBindBuffer(GL_ARRAY_BUFFER, *vbData->bufferId));
            u32 offset = 0;
            for (u8 i = 0; i < vbData->layoutCount; i++)
            {
                GL_ASSERT(glVertexAttribPointer(i, vbData->layout[i].elementCount,
                                                convertAtomicTypeToGLenum(vbData->layout[i].atomic),
                                                GL_FALSE, vertexSize, (void *)offset));
                GL_ASSERT(glEnableVertexAttribArray(i));
                offset += atomicTypeSize(vbData->layout[i].atomic) * vbData->layout[i].elementCount;
            }
            GL_ASSERT(glBindBuffer(GL_ARRAY_BUFFER, 0));
            GL_ASSERT(glBindVertexArray(0));

            return TRUE;
        }
        case GraphicsCommandType::DELETE_VERTEX_BUFFER:
        {
            DeleteVertexBufferCommandData *vbData = (DeleteVertexBufferCommandData *)command.data;
            GL_ASSERT(glDeleteBuffers(1, vbData->bufferId));
            GL_ASSERT(glDeleteVertexArrays(1, vbData->arrayId));
            return TRUE;
        }
        case GraphicsCommandType::DRAW_VERTEX_BUFFER:
        {
            DrawVertexBufferCommandData *drawData = (DrawVertexBufferCommandData *)command.data;
            GL_ASSERT(glBindVertexArray(drawData->bufferId));
            GL_ASSERT(glDrawArrays(GL_TRIANGLES, 0, drawData->count));
            GL_ASSERT(glBindVertexArray(0));
            return TRUE;
        }
        case GraphicsCommandType::CREATE_SHADER:
        {
            CreateShaderCommandData *shaderData = (CreateShaderCommandData *)command.data;

            GL_ASSERT(*shaderData->shaderId = glCreateShader(shaderData->type == ShaderType::VERTEX ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER));
            GL_ASSERT(glShaderSource(*shaderData->shaderId, 1, (const char **)&shaderData->shaderSource, (const GLint *)&shaderData->length));
            GL_ASSERT(glCompileShader(*shaderData->shaderId));

            GLint success;
            GLchar infoLog[512];
            glGetShaderiv(*shaderData->shaderId, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                GL_ASSERT(glGetShaderInfoLog(*shaderData->shaderId, 512, NULL, infoLog));
                print(infoLog, ConsoleColor::RED);
                GL_ASSERT(glDeleteShader(*shaderData->shaderId));
                *shaderData->shaderId = 0;
                return FALSE;
            }

            return TRUE;
        }
        case GraphicsCommandType::DELETE_SHADER:
        {
            DeleteShaderCommandData *deleteData = (DeleteShaderCommandData *)command.data;
            GL_ASSERT(glDeleteShader(deleteData->shaderId));
            return TRUE;
        }
        case GraphicsCommandType::CREATE_PIPELINE:
        {
            CreatePipelineCommandData *pipelineData = (CreatePipelineCommandData *)command.data;

            GL_ASSERT(*pipelineData->programId = glCreateProgram());
            GL_ASSERT(glAttachShader(*pipelineData->programId, pipelineData->vertexShaderId));
            GL_ASSERT(glAttachShader(*pipelineData->programId, pipelineData->fragmentShaderId));
            GL_ASSERT(glLinkProgram(*pipelineData->programId));

            GLint success;
            GLchar infoLog[512];
            glGetProgramiv(*pipelineData->programId, GL_LINK_STATUS, &success);
            if (!success)
            {
                GL_ASSERT(glGetProgramInfoLog(*pipelineData->programId, 512, NULL, infoLog));
                print(infoLog, ConsoleColor::RED);
                GL_ASSERT(glDeleteProgram(*pipelineData->programId));
                *pipelineData->programId = 0;
                return FALSE;
            }

            return TRUE;
        }
        case GraphicsCommandType::USE_PIPELINE:
        {
            UsePipelineCommandData *useData = (UsePipelineCommandData *)command.data;
            GL_ASSERT(glUseProgram(useData->programId));
            return TRUE;
        }
        default:
            return FALSE;
        }
    }
} // namespace rpp

#endif