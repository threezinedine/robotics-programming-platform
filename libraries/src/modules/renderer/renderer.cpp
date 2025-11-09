#include "modules/renderer/renderer_impl.h"
#include "core/containers/containers.h"
#include "modules/renderer/program.h"
#include "modules/renderer/rectangle.h"
#include "modules/renderer/imgui_internal.h"
#include "modules/renderer/line.h"
#include "modules/renderer/texture.h"
#include "modules/renderer/test_utils.h"
#include "plutovg.h"

#define MOUSE_CURSOR_SIZE (32.0f)

namespace rpp
{
    // TODO: Using command queue later.

    u32 Renderer::s_currentRendererIndex = INVALID_RENDERER_INDEX;
    Scope<Storage<Renderer::RendererData>> Renderer::s_currentRenderers = nullptr;

    void Renderer::Initialize()
    {
        RPP_PROFILE_SCOPE();

        RPP_ASSERT(s_currentRenderers == nullptr);
        if (!Graphics::Init())
        {
            throw std::runtime_error("Failed to initialize graphics backend");
        }

        s_currentRenderers = CreateScope<Storage<Renderer::RendererData>>();
        Program::Initialize();
        Texture::Initialize();
        Rectangle::Initialize();
        Line::Initialize();
        ImGuiImpl::Initialize();

#if defined(RPP_USE_TEST)
        TestUtils::Initialize();
#endif
        s_currentRendererIndex = INVALID_RENDERER_INDEX;
    }

    void Renderer::Shutdown()
    {
        RPP_PROFILE_SCOPE();

        RPP_ASSERT(s_currentRenderers != nullptr);

#if defined(RPP_USE_TEST)
        TestUtils::Shutdown();
#endif
        ImGuiImpl::Shutdown();
        Line::Shutdown();
        Rectangle::Shutdown();
        Program::Shutdown();
        Texture::Shutdown();
        s_currentRenderers.reset();
        Graphics::Shutdown();
    }

    void Renderer::PreDraw()
    {
        RPP_PROFILE_SCOPE();
        RendererData *current = GetCurrentRenderer();

        RPP_ASSERT(current != nullptr);
        RPP_ASSERT(current->imguiId != INVALID_ID);
        ImGuiImpl::PrepareFrame(current->imguiId);

        // Main loop
        current->window->PollEvents();

        // Clear color command
        ClearColorCommandData clearColorData = {{0.1f, 0.2f, 0.3f, 1.0f}};
        GraphicsCommandData commandData = {GraphicsCommandType::CLEAR_COLOR, &clearColorData};
        current->window->ExecuteCommand(commandData);
    }

    void Renderer::PostDraw()
    {
        RPP_PROFILE_SCOPE();
        // Currently nothing to do here.
    }

    void Renderer::Present()
    {
        RPP_PROFILE_SCOPE();
        RendererData *current = GetCurrentRenderer();

        RPP_ASSERT(current != nullptr);
        RPP_ASSERT(current->imguiId != INVALID_ID);
        ImGuiImpl::Render(current->imguiId);

#if defined(RPP_USE_TEST)
        TestUtils::Update(0.0f);
#endif

        GraphicsCommandData swapBuffersCommand = {GraphicsCommandType::SWAP_BUFFERS, nullptr};
        current->window->ExecuteCommand(swapBuffersCommand);
    }

    Renderer::RendererData *Renderer::GetCurrentRenderer()
    {
        RPP_PROFILE_SCOPE();
        RPP_ASSERT(s_currentRenderers != nullptr);
        RPP_ASSERT(s_currentRendererIndex != INVALID_RENDERER_INDEX);
        return s_currentRenderers->Get(s_currentRendererIndex);
    }

    u32 Renderer::Create(u32 width, u32 height, const String &title)
    {
        RPP_PROFILE_SCOPE();
        RPP_ASSERT(s_currentRenderers != nullptr);
        u32 rendererId = s_currentRenderers->Create();
        RendererData *currentRenderer = s_currentRenderers->Get(rendererId);

        RendererGraphicData graphicData = {};
        graphicData.rendererId = rendererId;

        currentRenderer->window = Graphics::CreateWindow(width, height, title.CStr(), &graphicData, sizeof(RendererGraphicData));

#if defined(RPP_USE_TEST) && 0
        {
            RPP_LOG_INFO("Disabling mouse input for renderer id {}", rendererId);
            DisableMouseCommandData disableMouseData = {};
            GraphicsCommandData commandData = {GraphicsCommandType::DISABLE_MOUSE, &disableMouseData};
            currentRenderer->window->ExecuteCommand(commandData);
        }
#endif

        Activate(rendererId);
        currentRenderer->rectangleId = Rectangle::Create();
        currentRenderer->lineId = Line::Create();

        // create rectangle mask texture
        {
            u32 maskWidth = 400;
            u32 maskHeight = 400;

            plutovg_surface_t *surface = plutovg_surface_create(maskWidth, maskHeight);
            plutovg_canvas_t *canvas = plutovg_canvas_create(surface);

            plutovg_canvas_save(canvas);
            plutovg_canvas_rect(canvas, 0, 0, maskWidth, maskHeight);
            plutovg_canvas_set_rgba(canvas, 1, 1, 1, 1);
            plutovg_canvas_fill(canvas);
            plutovg_canvas_restore(canvas);
            plutovg_canvas_save(canvas);

            currentRenderer->rectangleMask = Texture::Create(maskWidth, maskHeight, 4, (u8 *)plutovg_surface_get_data(surface));

            plutovg_canvas_destroy(canvas);
            plutovg_surface_destroy(surface);
        }

        // create circle mask texture
        {
            u32 maskWidth = 400;
            u32 maskHeight = 400;
            f32 centerX = maskWidth / 2.0f;
            f32 centerY = maskHeight / 2.0f;
            f32 radius = maskWidth / 2.0f;

            plutovg_surface_t *surface = plutovg_surface_create(maskWidth, maskHeight);
            plutovg_canvas_t *canvas = plutovg_canvas_create(surface);

            plutovg_canvas_save(canvas);
            plutovg_canvas_rect(canvas, 0, 0, maskWidth, maskHeight);
            plutovg_canvas_set_rgba(canvas, 0, 0, 0, 0);
            plutovg_canvas_fill(canvas);
            plutovg_canvas_restore(canvas);

            plutovg_canvas_arc(canvas, centerX, centerY, radius, 0, 2 * 3.1415926f, 0);
            plutovg_canvas_set_rgba(canvas, 1, 1, 1, 1);
            plutovg_canvas_fill(canvas);
            plutovg_canvas_restore(canvas);
            plutovg_canvas_save(canvas);

            currentRenderer->circleMask = Texture::Create(maskWidth, maskHeight, 4, (u8 *)plutovg_surface_get_data(surface));

            plutovg_canvas_destroy(canvas);
            plutovg_surface_destroy(surface);
        }

        // loading mouse
        {
            currentRenderer->mouseTexture = Texture::Create(STRINGIFY(RPP_PROJECT_DIR) "/assets/images/cursor.png");
            RPP_ASSERT(currentRenderer->mouseTexture != INVALID_ID);
        }

        ActivateContextCommandData activateContextData = {};
        GraphicsCommandData commandData = {GraphicsCommandType::ACTIVATE_CONTEXT, &activateContextData};
        currentRenderer->window->ExecuteCommand(commandData);

        currentRenderer->imguiId = ImGuiImpl::Create();

        return rendererId;
    }

    void Renderer::DrawRectangle(const Rect &rect, u32 textureId, u32 maskTextureId)
    {
        RPP_PROFILE_SCOPE();
        RPP_ASSERT(s_currentRenderers != nullptr);
        RPP_ASSERT(s_currentRendererIndex != INVALID_RENDERER_INDEX);
        RendererData *current = s_currentRenderers->Get(s_currentRendererIndex);
        u32 usedTextureId = textureId;

        if (usedTextureId == INVALID_ID)
        {
            usedTextureId = current->rectangleMask;
        }

        u32 usedMaskTextureId = maskTextureId;
        if (usedMaskTextureId == INVALID_ID)
        {
            usedMaskTextureId = current->rectangleMask;
        }

        Rectangle::Draw(GetCurrentRenderer()->rectangleId, rect, usedTextureId, usedMaskTextureId);
    }

    void Renderer::DrawCircle(const Point &center, f32 radius, u32 textureId)
    {
        RPP_PROFILE_SCOPE();
        RPP_ASSERT(s_currentRenderers != nullptr);
        RPP_ASSERT(s_currentRendererIndex != INVALID_RENDERER_INDEX);

        RendererData *current = s_currentRenderers->Get(s_currentRendererIndex);
        RPP_ASSERT(current != nullptr);

        u32 usedTextureId = textureId;
        if (usedTextureId == INVALID_ID)
        {
            usedTextureId = current->circleMask;
        }

        Rectangle::Draw(GetCurrentRenderer()->rectangleId,
                        Rect{center.x, center.y, radius * 2, radius * 2},
                        usedTextureId,
                        current->circleMask);
    }

    void Renderer::DrawLine(const Point &start, const Point &end)
    {
        RPP_PROFILE_SCOPE();
        Line::Draw(GetCurrentRenderer()->lineId, start, end);
    }

    void Renderer::Activate(u32 renderId)
    {
        RPP_PROFILE_SCOPE();
        RPP_ASSERT(s_currentRenderers != nullptr);
        RPP_ASSERT(s_currentRenderers->Get(renderId) != nullptr)

        s_currentRendererIndex = renderId;

        // Activate context command
        ActivateContextCommandData activateContextData = {};
        GraphicsCommandData commandData = {GraphicsCommandType::ACTIVATE_CONTEXT, &activateContextData};
        GetCurrentRenderer()->window->ExecuteCommand(commandData);
    }

    void Renderer::DrawMouseCursor(const Point &position)
    {
        RPP_PROFILE_SCOPE();
        RPP_ASSERT(s_currentRenderers != nullptr);
        RPP_ASSERT(s_currentRendererIndex != INVALID_RENDERER_INDEX);

        RendererData *current = s_currentRenderers->Get(s_currentRendererIndex);
        RPP_ASSERT(current != nullptr);
        RPP_ASSERT(current->mouseTexture != INVALID_ID);

        Renderer::DrawRectangle(Rect{position.x, position.y, MOUSE_CURSOR_SIZE, MOUSE_CURSOR_SIZE}, current->mouseTexture);
    }

    void Renderer::DrawingSceneInImGui()
    {
        RPP_PROFILE_SCOPE();
        RPP_ASSERT(GetCurrentRenderer()->imguiId != INVALID_ID);
        ImGuiImpl::DrawRenderingScene(GetCurrentRenderer()->imguiId);
    }

    void Renderer::CloseWindow()
    {
        RPP_PROFILE_SCOPE();
        RPP_ASSERT(s_currentRenderers != nullptr);
        RPP_ASSERT(s_currentRendererIndex != INVALID_RENDERER_INDEX);

        RendererData *current = s_currentRenderers->Get(s_currentRendererIndex);
        RPP_ASSERT(current != nullptr);

        CloseWindowCommandData closeWindowData = {};
        GraphicsCommandData commandData = {GraphicsCommandType::CLOSE_WINDOW, &closeWindowData};
        current->window->ExecuteCommand(commandData);
    }

    void Renderer::SetWindowTitle(const String &title)
    {
        RPP_PROFILE_SCOPE();
        RPP_ASSERT(s_currentRenderers != nullptr);
        RPP_ASSERT(s_currentRendererIndex != INVALID_RENDERER_INDEX);

        RendererData *current = s_currentRenderers->Get(s_currentRendererIndex);
        RPP_ASSERT(current != nullptr);

        current->window->SetWindowTitle(title.CStr());
    }

    void Renderer::Destroy(u32 renderId)
    {
        RPP_PROFILE_SCOPE();
        RPP_ASSERT(s_currentRenderers != nullptr);
        RPP_ASSERT(renderId != INVALID_RENDERER_INDEX);

        RendererData *data = s_currentRenderers->Get(renderId);

        RPP_ASSERT(data != nullptr);
        RPP_ASSERT(data->imguiId != INVALID_ID);

        ImGuiImpl::Destroy(data->imguiId);
        Texture::Destroy(data->circleMask);
        Texture::Destroy(data->rectangleMask);
        Texture::Destroy(data->mouseTexture);

        Rectangle::Destroy(data->rectangleId);
        Line::Destroy(data->lineId);

        s_currentRenderers->Free(renderId);
        if (s_currentRendererIndex == renderId)
        {
            s_currentRendererIndex = INVALID_RENDERER_INDEX;
        }
    }
} // namespace rpp
