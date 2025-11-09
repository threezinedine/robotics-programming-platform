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

#include "modules/input/input.h"
#include "modules/renderer/test_utils.h"

namespace rpp
{
#define INVALID_IMGUI_KEY ImGuiKey_None

    static ImGuiKey s_imguiKeyMap[KeyboardButton::KEYBOARD_BUTTON_COUNT] = {
        ImGuiKey_A,
        ImGuiKey_B,
        ImGuiKey_C,
        ImGuiKey_D,
        ImGuiKey_E,
        ImGuiKey_F,
        ImGuiKey_G,
        ImGuiKey_H,
        ImGuiKey_I,
        ImGuiKey_J,
        ImGuiKey_K,
        ImGuiKey_L,
        ImGuiKey_M,
        ImGuiKey_N,
        ImGuiKey_O,
        ImGuiKey_P,
        ImGuiKey_Q,
        ImGuiKey_R,
        ImGuiKey_S,
        ImGuiKey_T,
        ImGuiKey_U,
        ImGuiKey_V,
        ImGuiKey_W,
        ImGuiKey_X,
        ImGuiKey_Y,
        ImGuiKey_Z,

        ImGuiKey_0,
        ImGuiKey_1,
        ImGuiKey_2,
        ImGuiKey_3,
        ImGuiKey_4,
        ImGuiKey_5,
        ImGuiKey_6,
        ImGuiKey_7,
        ImGuiKey_8,
        ImGuiKey_9,
        ImGuiKey_Escape,
        INVALID_IMGUI_KEY, // ImGuiKey_LCtrl,
        INVALID_IMGUI_KEY, // ImGuiKey_LShift,
        INVALID_IMGUI_KEY, // ImGuiKey_LAlt,
        INVALID_IMGUI_KEY, // ImGuiKey_LSystem,
        INVALID_IMGUI_KEY, // ImGuiKey_RCtrl,
        INVALID_IMGUI_KEY, // ImGuiKey_RShift,
        INVALID_IMGUI_KEY, // ImGuiKey_RAlt,
        INVALID_IMGUI_KEY, // ImGuiKey_RSystem,
        ImGuiKey_Menu,

        ImGuiKey_LeftBracket,
        ImGuiKey_RightBracket,
        ImGuiKey_Semicolon,
        ImGuiKey_Comma,
        ImGuiKey_Period,
        INVALID_IMGUI_KEY, // ImGuiKey_Quote,
        ImGuiKey_Slash,
        ImGuiKey_Backslash,
        INVALID_IMGUI_KEY, // ImGuiKey_Tilde,
        ImGuiKey_Equal,
        INVALID_IMGUI_KEY, // ImGuiKey_Dash,
        ImGuiKey_Space,
        INVALID_IMGUI_KEY, // ImGuiKey_Return,
        ImGuiKey_Backspace,
        ImGuiKey_Tab,
        ImGuiKey_PageUp,
        ImGuiKey_PageDown,
        ImGuiKey_End,
        ImGuiKey_Home,
        ImGuiKey_Insert,
        ImGuiKey_Delete,
        INVALID_IMGUI_KEY, // ImGuiKey_Add,
        INVALID_IMGUI_KEY, // ImGuiKey_Subtract,
        INVALID_IMGUI_KEY, // ImGuiKey_Multiply,
        INVALID_IMGUI_KEY, // ImGuiKey_Divide,
        INVALID_IMGUI_KEY, // ImGuiKey_Left,
        INVALID_IMGUI_KEY, // ImGuiKey_Right,
        INVALID_IMGUI_KEY, // ImGuiKey_Up,
        INVALID_IMGUI_KEY, // ImGuiKey_Down,
        INVALID_IMGUI_KEY, // ImGuiKey_Numpad0,
        INVALID_IMGUI_KEY, // ImGuiKey_Numpad1,
        INVALID_IMGUI_KEY, // ImGuiKey_Numpad2,
        INVALID_IMGUI_KEY, // ImGuiKey_Numpad3,
        INVALID_IMGUI_KEY, // ImGuiKey_Numpad4,
        INVALID_IMGUI_KEY, // ImGuiKey_Numpad5,
        INVALID_IMGUI_KEY, // ImGuiKey_Numpad6,
        INVALID_IMGUI_KEY, // ImGuiKey_Numpad7,
        INVALID_IMGUI_KEY, // ImGuiKey_Numpad8,
        INVALID_IMGUI_KEY, // ImGuiKey_Numpad9,
        ImGuiKey_F1,
        ImGuiKey_F2,
        ImGuiKey_F3,
        ImGuiKey_F4,
        ImGuiKey_F5,
        ImGuiKey_F6,
        ImGuiKey_F7,
        ImGuiKey_F8,
        ImGuiKey_F9,
        ImGuiKey_F10,
        ImGuiKey_F11,
        ImGuiKey_F12,
    };

    Scope<Storage<ImGuiImpl::ImGuiData>>
        ImGuiImpl::s_imguis = nullptr;

    void ImGuiImpl::Initialize()
    {
        RPP_PROFILE_SCOPE();
        RPP_ASSERT(s_imguis == nullptr);

        auto ImGuiDeallocator = [](ImGuiData *pData)
        {
            // Cleanup
            RPP_ASSERT(pData != nullptr);

            DeleteFrameBufferCommandData fbData = {};
            fbData.frameBufferId = pData->frameBufferId;
            fbData.textureId = pData->textureId;
            fbData.renderBufferId = pData->frameRenderBufferId;

            ImGui::DestroyContext(pData->context);

            GraphicsCommandData commandData = {GraphicsCommandType::DELETE_FRAMEBUFFER, &fbData};
            Renderer::GetWindow()->ExecuteCommand(commandData);
            RPP_DELETE(pData);
        };

        s_imguis = CreateScope<Storage<ImGuiData>>();
    }

    void ImGuiImpl::Shutdown()
    {
        RPP_PROFILE_SCOPE();
        RPP_ASSERT(s_imguis != nullptr);

        s_imguis.reset();
    }

    u32 ImGuiImpl::Create()
    {
        RPP_PROFILE_SCOPE();
        RPP_ASSERT(s_imguis != nullptr);

        u32 id = s_imguis->Create();
        ImGuiData *pData = s_imguis->Get(id);
        pData->rendererId = Renderer::GetCurrentRendererId();

        IMGUI_CHECKVERSION();
        pData->context = ImGui::CreateContext();
        ImGui::SetCurrentContext(pData->context);
        ImGuiIO &io = ImGui::GetIO();
        pData->io = &io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking
        // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;   // Enable Multi-Viewport / Platform Windows
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

#if 0
        // Setup Fonts
        {
            ImFontConfig fontConfig;
            fontConfig.FontDataOwnedByAtlas = false;
            io.Fonts->AddFontFromMemoryTTF((void *)tahoma, sizeof(tahoma), 17.0f, &fontConfig);
            ImGui::MergeIconsWithLatestFont(16.0f, false);
        }
#endif

        CreateFrameBufferCommandData fbData;
        fbData.width = 800;
        fbData.height = 600;
        fbData.pFrameBufferId = &pData->frameBufferId;
        fbData.pTextureId = &pData->textureId;
        fbData.pRenderBufferId = &pData->frameRenderBufferId;

        GraphicsCommandData commandData{GraphicsCommandType::CREATE_FRAMEBUFFER, &fbData};
        Renderer::GetWindow()->ExecuteCommand(commandData);

        Renderer::GetWindow()->SetResizeCallback(
            [](u32 width, u32 height, void *pUserData)
            {
                RendererGraphicData *graphicData = (RendererGraphicData *)pUserData;
                ResizeFrameBufferCommandData resizeData;
                Renderer::RendererData *rendererData = Renderer::s_currentRenderers->Get(graphicData->rendererId);
                ImGuiImpl::ImGuiData *imguiData = ImGuiImpl::s_imguis->Get(rendererData->imguiId);

                resizeData.frameBufferId = imguiData->frameBufferId;
                resizeData.renderBufferId = imguiData->frameRenderBufferId;
                resizeData.textureId = imguiData->textureId;
                resizeData.width = width;
                resizeData.height = height;
                GraphicsCommandData commandData = {GraphicsCommandType::RESIZE_FRAMEBUFFER, &resizeData};
                Renderer::GetWindow()->ExecuteCommand(commandData);
            });

#if 0
        Renderer::GetWindow()->SetOnMouseMoveCallback(
            [](f64 xPos, f64 yPos, void *pUserData)
            {
                InputSystem::OnMouseMoveCallback(xPos, yPos, pUserData);
            });
#endif

        return id;
    }

    void ImGuiImpl::PrepareFrame(u32 imguiId)
    {
        RPP_PROFILE_SCOPE();
        RPP_ASSERT(s_imguis != nullptr);
        ImGuiData *pData = s_imguis->Get(imguiId);
        RPP_ASSERT(pData != nullptr);

        RPP_ASSERT(pData->rendererId == Renderer::GetCurrentRendererId());

        // Start the Dear ImGui frame
        ImGui::SetCurrentContext(pData->context);
#if defined(RPP_USE_TEST)
        // sync mouse position
        {
            ImGuiIO &io = ImGui::GetIO();
            io.MousePos = ImVec2(io.MousePos.x, io.MousePos.y);
            io.AddMousePosEvent(f32(InputSystem::GetMouseX()), f32(InputSystem::GetMouseY()));

            for (i32 button = 0; button < MouseButton::MOUSE_BUTTON_COUNT; ++button)
            {
                b8 isPressed = InputSystem::IsMouseButtonDown(static_cast<MouseButton>(button));
                io.MouseClicked[button] = isPressed;
                io.AddMouseButtonEvent(button, isPressed);
            }

#if 1
            for (i32 key = 0; key < KeyboardButton::KEYBOARD_BUTTON_COUNT; ++key)
            {
                b8 isPressed = InputSystem::IsKeyboardButtonDown(static_cast<KeyboardButton>(key));

                ImGuiKey keyEnum = s_imguiKeyMap[key];
                if (keyEnum == INVALID_IMGUI_KEY)
                {
                    continue;
                }

                // io.KeysData[keyEnum].Down = false;
                io.AddKeyEvent(keyEnum, isPressed);
            }
#endif
        }
#endif
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());

        // Bind framebuffer
        BindFrameBufferCommandData bindData;
        bindData.frameBufferId = pData->frameBufferId;

        GraphicsCommandData commandData = {GraphicsCommandType::BIND_FRAMEBUFFER, &bindData};
        Renderer::GetWindow()->ExecuteCommand(commandData);
    }

    void ImGuiImpl::Render(u32 imguiId)
    {
        RPP_PROFILE_SCOPE();
        // Unbind framebuffer
        UnbindFrameBufferCommandData unbindData;
        GraphicsCommandData unbindCommandData = {GraphicsCommandType::UNBIND_FRAMEBUFFER, &unbindData};
        Renderer::GetWindow()->ExecuteCommand(unbindCommandData);

        RPP_ASSERT(s_imguis != nullptr);
        ImGuiData *data = s_imguis->Get(imguiId);
        RPP_ASSERT(data != nullptr);
        RPP_ASSERT(data->rendererId == Renderer::GetCurrentRendererId() && "ImGui instance was created with a different renderer!");

#if defined(RPP_USE_TEST)
        // Drawing the cursor for testing
        {
            ImDrawList *pDrawList = ImGui::GetForegroundDrawList();
            ImVec2 mousePos = ImGui::GetIO().MousePos;

            pDrawList->AddImage(
                (void *)(intptr_t)Renderer::GetCurrentRenderer()->circleMask,
                ImVec2(mousePos.x, mousePos.y),
                ImVec2(mousePos.x, mousePos.y));
        }

        // Renderer::DrawMouseCursor({f32(InputSystem::GetMouseX()), f32(InputSystem::GetMouseY())});
#endif

#if 0
        // Toast notifications
        {
            // Set position
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.f);
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.7, 0.7, 0.7, 0.9));
            ImGui::RenderNotifications();
            ImGui::PopStyleVar(1);
            ImGui::PopStyleColor(1);
        }
#endif

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
        RPP_PROFILE_SCOPE();
        ImGui::BeginChild("RenderingScene");
        float displayWidth = ImGui::GetContentRegionAvail().x;
        float displayHeight = ImGui::GetContentRegionAvail().y;

        RPP_ASSERT(s_imguis != nullptr);
        ImGuiData *data = s_imguis->Get(imguiId);
        RPP_ASSERT(data != nullptr);

        ImGui::Image((void *)(uintptr_t)data->textureId, ImVec2(displayWidth, displayHeight), ImVec2(0, 1), ImVec2(1, 0));
        ImGui::EndChild();
    }

    void ImGuiImpl::LabelItem(const String &label, ImGuiID imguiId)
    {
        RPP_PROFILE_SCOPE();
        ImGuiImpl::LabelItem(label, nullptr, imguiId);
    }

    void ImGuiImpl::LabelItem(const String &label, void *pData, ImGuiID imguiId)
    {
        RPP_PROFILE_SCOPE();
#if defined(RPP_USE_TEST)
        TestUtils::GlobalData *pTestData = TestUtils::s_pData;
        RPP_ASSERT(pTestData != nullptr);

        if (TestUtils::s_itemFound || pTestData->label != label)
        {
            return;
        }

        TestUtils::s_pCurrentItemData = RPP_NEW(TestUtils::ItemData);
        TestUtils::ItemData *pCurrentData = TestUtils::s_pCurrentItemData;
        pCurrentData->label = label;
        pCurrentData->position = ImGui::GetItemRectMin();
        pCurrentData->rendererId = Renderer::GetCurrentRendererId();

        RPP_ASSERT(pCurrentData->rendererId != INVALID_ID);
        TestUtils::s_itemFound = TRUE;

        if (pTestData->labelCheckingCallback)
        {
            pTestData->labelCheckingCallback(label, pData, imguiId);
        }
#else
        RPP_UNUSED(label);
        RPP_UNUSED(pData);
        RPP_UNUSED(imguiId);
#endif
    }

    void ImGuiImpl::Destroy(u32 imguiId)
    {
        RPP_PROFILE_SCOPE();
        RPP_ASSERT(s_imguis != nullptr);
        s_imguis->Free(imguiId);
    }
} // namespace rpp
