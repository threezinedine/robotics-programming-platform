#pragma once
#include "platforms/common.h"

namespace rpp
{
    /**
     * @brief All types of actions that the graphics backend can perform.
     */
    enum class GraphicsCommandType
    {
        CLEAR_COLOR,      ///< Specify the screen color.
        ACTIVATE_CONTEXT, ///< Activate the rendering context.
        SWAP_BUFFERS,     ///< Swap the front and back buffers.
        RESET_VIEWPORT,   ///< Reset the viewport to the entire window.

        CREATE_VERTEX_BUFFER, ///< Create a vertex buffer.
        DELETE_VERTEX_BUFFER, ///< Delete a vertex buffer.
        DRAW_VERTEX_BUFFER,   ///< Draw a vertex buffer.

        CREATE_SHADER, ///< Create a shader program.
        DELETE_SHADER, ///< Delete a shader program.

        CREATE_PIPELINE, ///< Create a rendering pipeline.
        DELETE_PIPELINE, ///< Delete a rendering pipeline.

        USE_PIPELINE, ///< Use a rendering pipeline.

        SET_UNIFORM, ///< Set a uniform variable in the shader program.

        CREATE_TEXTURE, ///< Create a 2D texture.
        DELETE_TEXTURE, ///< Delete a 2D texture.
        ACTIVE_TEXTURE, ///< Activate a texture unit.

        CREATE_FRAMEBUFFER, ///< Create a framebuffer object.
        DELETE_FRAMEBUFFER, ///< Delete a framebuffer object.
        RESIZE_FRAMEBUFFER, ///< Resize a framebuffer object.
        BIND_FRAMEBUFFER,   ///< Bind a framebuffer object.
        UNBIND_FRAMEBUFFER, ///< Unbind the currently bound framebuffer object.
        COUNT,
    };

    /**
     * @brief The data structure for a graphics command which will be send to the graphics backend. (platform queue)
     */
    struct GraphicsCommandData
    {
        GraphicsCommandType type; ///< The type of the command.
        void *pData;              ///< The data of the command.
    };

    struct ClearColorCommandData
    {
        f32 color[4]; ///< The RGBA color values.
    };

    struct ResetViewportCommandData
    {
    };

    struct ActivateContextCommandData
    {
    };

    /**
     * @brief The data structure for the create texture command.
     */
    struct CreateTextureCommandData
    {
        u32 width;           ///< The width of the texture.
        u32 height;          ///< The height of the texture.
        u8 numberOfChannels; ///< The number of color channels (e.g., 3 for RGB, 4 for RGBA).

        u8 *pPixelData;  ///< Pointer to the pixel data.
        u32 *pTextureId; ///< Pointer to store the generated texture ID.
    };

    /**
     * @brief The data structure for the delete texture command.
     */
    struct DeleteTextureCommandData
    {
        u32 textureId; ///< The ID of the texture to be deleted.
    };

    /**
     * @brief The data structure for the active texture command.
     */
    struct ActiveTextureCommandData
    {
        u32 slot;      ///< The texture slot to activate (e.g., 0 for GL_TEXTURE0).
        u32 textureId; ///< The ID of the texture to bind to the active slot.
    };

    /**
     * @brief Specifies how the vertices are related.
     */
    enum class VertexBufferType : u8
    {
        LINE,           ///< Each pair of vertices forms a line.
        LINE_STRIP,     ///< A connected group of lines. Each vertex (after the first) connects to the previous vertex.
        TRIANGLE,       ///< Each triplet of vertices forms a triangle.
        TRIANGLE_STRIP, ///< A connected group of triangles. Each vertex (after the first two) connects to the previous two vertices.
        COUNT,
    };

    enum class AtomicType : u8
    {
        FLOAT,  ///< 32-bit floating point.
        INT,    ///< 32-bit integer.
        UINT,   ///< 32-bit unsigned integer.
        DOUBLE, ///< 64-bit floating point.
        COUNT,
    };

    /**
     * @brief Describes the binding data of a single vertex attribute (e.g., position, color, texture coordinates).
     */
    struct LayoutElement
    {
        AtomicType atomic; ///< The size of a single component in bytes (e.g., 4 bytes for float).
        u8 elementCount;   ///< The number of components in the attribute (e.g., 3 for vec3).
    };

    /**
     * @brief The data structure for the create vertex buffer command.
     */
    struct CreateVertexBufferCommandData
    {
        u32 count;              ///< The number of vertices.
        void *pData;            ///< Pointer to the vertex data.
        u32 *pBufferId;         ///< Pointer to store the generated buffer ID.
        u32 *pArrayId;          ///< Pointer to store the generated vertex array ID.
        VertexBufferType type;  ///< The type of the vertex buffer.
        u8 layoutCount;         ///< The number of layout elements.
        LayoutElement *pLayout; ///< Pointer to the array of layout elements.
    };

    /**
     * @brief The data structure for the delete vertex buffer command.
     */
    struct DeleteVertexBufferCommandData
    {
        u32 *pBufferId; ///< The ID of the vertex buffer to be deleted.
        u32 *pArrayId;  ///< The ID of the vertex array to be deleted.
    };

    struct DrawVertexBufferCommandData
    {
        u32 bufferId;          ///< The ID of the vertex buffer to be drawn.
        VertexBufferType type; ///< The type of the vertex buffer.
        u32 count;             ///< The number of vertices to draw.
    };

    /**
     * @brief The types of shaders supported.
     */
    enum class ShaderType : u8
    {
        VERTEX,   ///< Vertex shader type.
        FRAGMENT, ///< Fragment shader type.
        COUNT,
    };

    struct CreateShaderCommandData
    {
        u8 *pShaderSource; ///< The source code of the vertex shader.
        u32 length;        ///< The length of the shader source code.
        ShaderType type;   ///< The type of the shader (vertex or fragment).

        u32 *pShaderId; ///< Pointer to store the generated shader program ID.
    };

    struct DeleteShaderCommandData
    {
        u32 shaderId; ///< The ID of the shader program to be deleted.
    };

    struct CreatePipelineCommandData
    {
        u32 vertexShaderId;   ///< The ID of the vertex shader.
        u32 fragmentShaderId; ///< The ID of the fragment shader.

        u32 *pProgramId; ///< Pointer to store the generated pipeline ID.
    };

    struct DeletePipelineCommandData
    {
        u32 programId; ///< The ID of the pipeline to be deleted.
    };

    struct UsePipelineCommandData
    {
        u32 programId; ///< The ID of the pipeline to be used.
    };

    /**
     * @brief The types of uniform variables supported.
     */
    enum class UniformType : u8
    {
        FLOAT, ///< 32-bit floating point.
        INT,   ///< 32-bit integer.
        UINT,  ///< 32-bit unsigned integer.

        VEC2, ///< 2-component vector of 32-bit floats.
        VEC3, ///< 3-component vector of 32-bit floats.
        VEC4, ///< 4-component vector of 32-bit floats.

        IVEC2, ///< 2-component vector of 32-bit unsigned integers.
        IVEC3, ///< 3-component vector of 32-bit unsigned integers.
        IVEC4, ///< 4-component vector of 32-bit unsigned integers.

        MAT2x2, ///< 2x2 matrix of 32-bit floats.
        MAT3x3, ///< 3x3 matrix of 32-bit floats.
        MAT4x4, ///< 4x4 matrix of 32-bit floats.

        MAT2x3, ///< 2x3 matrix of 32-bit floats.
        MAT2x4, ///< 2x4 matrix of 32-bit floats.
        MAT3x2, ///< 3x2 matrix of 32-bit floats.
        MAT3x4, ///< 3x4 matrix of 32-bit floats.
        MAT4x2, ///< 4x2 matrix of 32-bit floats.
        MAT4x3, ///< 4x3 matrix of 32-bit floats.

        COUNT,
    };

    struct UniformDescription
    {
        const char *name; ///< The name of the uniform variable in the shader.
        UniformType type; ///< The type of the uniform variable.
        void *pData;      ///< Pointer to the data to be set for the uniform variable.
    };

    /**
     * @brief The data structure for the set uniform command.
     * @note This is a simplified example. In a real implementation, you might want to support more types and array uniforms.
     */
    struct SetUniformCommandData
    {
        u32 programId;                 ///< The ID of the program where the uniform is located.
        u8 uniformCount;               ///< The number of uniforms to set.
        UniformDescription *pUniforms; ///< Pointer to the array of uniform descriptions.
    };

    struct CreateFrameBufferCommandData
    {
        u32 width;            ///< The width of the framebuffer.
        u32 height;           ///< The height of the framebuffer.
        u32 *pFrameBufferId;  ///< Pointer to store the generated framebuffer ID.
        u32 *pTextureId;      ///< Pointer to store the texture ID associated with the framebuffer.
        u32 *pRenderBufferId; ///< Pointer to store the renderbuffer ID associated with the framebuffer.
    };

    struct DeleteFrameBufferCommandData
    {
        u32 frameBufferId;  ///< The ID of the framebuffer to be deleted.
        u32 textureId;      ///< The ID of the texture associated with the framebuffer to be deleted.
        u32 renderBufferId; ///< The ID of the renderbuffer associated with the framebuffer to be deleted.
    };

    struct ResizeFrameBufferCommandData
    {
        u32 frameBufferId;  ///< The ID of the framebuffer to be resized.
        u32 renderBufferId; ///< The ID of the renderbuffer associated with the framebuffer to be resized.
        u32 textureId;      ///< The ID of the texture associated with the framebuffer to be resized.
        u32 width;          ///< The new width of the framebuffer.
        u32 height;         ///< The new height of the framebuffer.
    };

    struct BindFrameBufferCommandData
    {
        u32 frameBufferId; ///< The ID of the framebuffer to be bound.
    };

    struct UnbindFrameBufferCommandData
    {
    };
} // namespace rpp
