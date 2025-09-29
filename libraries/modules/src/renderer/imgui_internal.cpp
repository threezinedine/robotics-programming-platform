#include "modules/renderer/imgui_internal.h"
#include "modules/renderer/renderer_impl.h"
#include "imgui.h"
#include "modules/renderer/renderer.h"

#if defined(RPP_GRAPHICS_BACKEND_OPENGL)
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h> // Will drag system OpenGL headers
#else
#error "ImGui is only supported with OpenGL backend!"
#endif

namespace rpp
{
    Scope<Storage<ImGuiImpl::ImGuiData>>
        ImGuiImpl::s_imguis = nullptr;

    void ImGuiImpl::Initialize()
    {
        RPP_ASSERT(s_imguis == nullptr);

        s_imguis = CreateScope<Storage<ImGuiData>>();
    }

    void ImGuiImpl::Shutdown()
    {
        RPP_ASSERT(s_imguis != nullptr);

        s_imguis.reset();
    }

    u32 ImGuiImpl::Create()
    {
        RPP_ASSERT(s_imguis != nullptr);

        u32 id = s_imguis->Create();
        ImGuiData *data = s_imguis->Get(id);
        data->rendererId = Renderer::GetCurrentRendererId();

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        data->io = &io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;   // Enable Multi-Viewport / Platform Windows
        // io.ConfigViewportsNoAutoMerge = true;
        // io.ConfigViewportsNoTaskBarIcon = true;

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        // ImGui::StyleColorsLight();

        float main_scale = ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor()); // Valid on GLFW 3.3+ only
        // Setup scaling
        ImGuiStyle &style = ImGui::GetStyle();
        style.ScaleAllSizes(main_scale); // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
        style.FontScaleDpi = main_scale; // Set initial font scale. (using io.ConfigDpiScaleFonts=true makes this unnecessary. We leave both here for documentation purpose)
#if GLFW_VERSION_MAJOR >= 3 && GLFW_VERSION_MINOR >= 3
        io.ConfigDpiScaleFonts = true;     // [Experimental] Automatically overwrite style.FontScaleDpi in Begin() when Monitor DPI changes. This will scale fonts but _NOT_ scale sizes/padding for now.
        io.ConfigDpiScaleViewports = true; // [Experimental] Scale Dear ImGui and Platform Windows when Monitor DPI changes.
#endif

        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(glfwGetCurrentContext(), true);
#ifdef __EMSCRIPTEN__
        ImGui_ImplGlfw_InstallEmscriptenCallbacks(window, "#canvas");
#endif
        ImGui_ImplOpenGL3_Init("#version 130");

        CreateFrameBufferCommandData fbData;
        fbData.width = 800;
        fbData.height = 600;
        fbData.pFrameBufferId = &data->frameBufferId;
        fbData.pTextureId = &data->textureId;
        fbData.pRenderBufferId = &data->frameRenderBufferId;

        GraphicsCommandData commandData{GraphicsCommandType::CREATE_FRAMEBUFFER, &fbData};
        Renderer::GetWindow()->ExecuteCommand(commandData);

        return id;
    }

    void ImGuiImpl::PrepareFrame(u32 imguiId)
    {
        RPP_ASSERT(s_imguis != nullptr);
        ImGuiData *data = s_imguis->Get(imguiId);
        RPP_ASSERT(data != nullptr);

        RPP_ASSERT(data->rendererId == Renderer::GetCurrentRendererId() && "ImGui instance was created with a different renderer!");

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());

        // Bind framebuffer
        BindFrameBufferCommandData bindData;
        bindData.frameBufferId = data->frameBufferId;

        GraphicsCommandData commandData = {GraphicsCommandType::BIND_FRAMEBUFFER, &bindData};
        Renderer::GetWindow()->ExecuteCommand(commandData);
    }

    void ImGuiImpl::Render(u32 imguiId)
    {
        // Unbind framebuffer
        UnbindFrameBufferCommandData unbindData;
        GraphicsCommandData unbindCommandData = {GraphicsCommandType::UNBIND_FRAMEBUFFER, &unbindData};
        Renderer::GetWindow()->ExecuteCommand(unbindCommandData);

        RPP_ASSERT(s_imguis != nullptr);
        ImGuiData *data = s_imguis->Get(imguiId);
        RPP_ASSERT(data != nullptr);
        RPP_ASSERT(data->rendererId == Renderer::GetCurrentRendererId() && "ImGui instance was created with a different renderer!");

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(glfwGetCurrentContext(), &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Update and Render additional Platform Windows
        // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
        //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
        if (data->io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow *backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    }

    void ImGuiImpl::DrawRenderingScene(u32 imguiId)
    {
        ImGui::BeginChild("RenderingScene");
        float displayWidth = ImGui::GetContentRegionAvail().x;
        float displayHeight = ImGui::GetContentRegionAvail().y;

        RPP_ASSERT(s_imguis != nullptr);
        ImGuiData *data = s_imguis->Get(imguiId);
        RPP_ASSERT(data != nullptr);

        ImGui::Image((void *)(uintptr_t)data->textureId, ImVec2(displayWidth, displayHeight), ImVec2(0, 1), ImVec2(1, 0));
        ImGui::EndChild();
    }

    void ImGuiImpl::Destroy(u32 imguiId)
    {
        RPP_ASSERT(s_imguis != nullptr);
        s_imguis->Free(imguiId);

        ImGuiData *data = s_imguis->Get(imguiId);

        DeleteFrameBufferCommandData fbData = {};
        fbData.frameBufferId = data->frameBufferId;
        fbData.textureId = data->textureId;
        fbData.renderBufferId = data->frameRenderBufferId;

        GraphicsCommandData commandData = {GraphicsCommandType::DELETE_FRAMEBUFFER, &fbData};
        Renderer::GetWindow()->ExecuteCommand(commandData);
    }
} // namespace rpp
