#if defined(RPP_USE_TEST)
#include "modules/renderer/test_utils.h"
#include "modules/input/input.h"
#include "modules/test_system/test_system.h"

namespace rpp
{
    TestUtils::GlobalData *TestUtils::s_pData = nullptr;
    TestUtils::ItemData *TestUtils::s_pCurrentItemData = nullptr;
    b8 TestUtils::s_itemFound = FALSE;
    u8 TestUtils::s_findingFrameCount = 0;
    u32 TestUtils::s_focusedRendererId = INVALID_ID;

    void TestUtils::Initialize()
    {
        RPP_ASSERT(s_pData == nullptr);
        RPP_ASSERT(s_pCurrentItemData == nullptr);
        s_pData = RPP_NEW(TestUtils::GlobalData);
    }

    void TestUtils::Shutdown()
    {
        RPP_ASSERT(s_pData != nullptr);
        RPP_DELETE(s_pData);
    }

    void TestUtils::Update(f32 deltaTime)
    {
        RPP_UNUSED(deltaTime);

        if (s_pData->action == ImGuiItemAction::IMGUI_ACTION_COUNT)
        {
            return;
        }

        if (s_findingFrameCount == 0)
        {
            return;
        }

        switch (s_pData->action)
        {
        case ImGuiItemAction::IMGUI_ACTION_MOVE:
        {
            if (s_itemFound)
            {
                RPP_ASSERT(s_pCurrentItemData != nullptr);

                if (s_pCurrentItemData->rendererId != Renderer::GetCurrentRendererId())
                {
                    break;
                }

                b8 arrived = InputSystem::MoveMouseTo(s_pCurrentItemData->position.x, s_pCurrentItemData->position.y);

                if (arrived)
                {
                    TestSystem::GetInstance()->SetMainThreadWorking(FALSE);
                    ResetCurrentItem();
                }
            }
            else
            {
                REGISTER_ERROR("Cannot find ImGui item with label '{}'", s_pData->label);
                TestSystem::GetInstance()->SetMainThreadWorking(FALSE);
                ResetCurrentItem();
            }
            break;
        }
        case ImGuiItemAction::IMGUI_ACTION_FIND_ITEM:
            break;
        case ImGuiItemAction::IMGUI_ACTION_ASSERT_TEXT_INPUT:
        {
            TestSystem::GetInstance()->SetMainThreadWorking(FALSE);
            if (!s_itemFound)
            {
                REGISTER_ERROR("Cannot find ImGui item with label '{}'", s_pData->label);
                ResetCurrentItem();
            }

            break;
        }
        case ImGuiItemAction::IMGUI_ACTION_CLICK:
        {
            if (s_itemFound)
            {
                RPP_ASSERT(s_pCurrentItemData != nullptr);

                if (s_pCurrentItemData->rendererId != Renderer::GetCurrentRendererId())
                {
                    break;
                }

                b8 arrived = InputSystem::MoveMouseTo(s_pCurrentItemData->position.x, s_pCurrentItemData->position.y);

                if (!arrived)
                {
                    break;
                }

                b8 done = InputSystem::ClickMouse(0);

                s_focusedRendererId = Renderer::GetCurrentRendererId();

                if (done)
                {
                    TestSystem::GetInstance()->SetMainThreadWorking(FALSE);
                    ResetCurrentItem();
                }
            }
            else
            {
                REGISTER_ERROR("Cannot find ImGui item with label '{}'", s_pData->label);
                TestSystem::GetInstance()->SetMainThreadWorking(FALSE);
                ResetCurrentItem();
            }
            break;
        }
        case ImGuiItemAction::IMGUI_ACTION_TYPE:
        {
            if (s_focusedRendererId != Renderer::GetCurrentRendererId())
            {
                break;
            }

            for (i32 i = 0; i < s_pData->text.Length(); i++)
            {
                ImGuiIO &io = ImGui::GetIO();
                io.AddInputCharacter(static_cast<u32>(s_pData->text[i]));
            }

            TestSystem::GetInstance()->SetMainThreadWorking(FALSE);
            ResetCurrentItem();

            break;
        }
        case ImGuiItemAction::IMGUI_ACTION_ENTER:
        {
            b8 complete = InputSystem::Press(KeyboardButton::KEY_RETURN);

            if (!complete)
            {
                break;
            }

            TestSystem::GetInstance()->SetMainThreadWorking(FALSE);
            ResetCurrentItem();

            break;
        }
        default:
            RPP_UNREACHABLE();
        }
    }

    void TestUtils::ResetCurrentItem()
    {
        s_pData->label = "";
        s_pData->action = ImGuiItemAction::IMGUI_ACTION_COUNT;
        s_pData->text = "";
        s_pData->characterIndex = 0;
        s_pData->labelCheckingCallback = nullptr;
        s_findingFrameCount = 0;
        s_itemFound = FALSE;

        if (s_pCurrentItemData != nullptr)
        {
            RPP_DELETE(s_pCurrentItemData);
            s_pCurrentItemData = nullptr;
        }
    }

    void TestUtils::MoveToItem(const String &label)
    {
        RPP_ASSERT(s_pData != nullptr);
        s_pData->label = label;
        s_pData->action = ImGuiItemAction::IMGUI_ACTION_MOVE;
        s_pData->text = "";
        s_pData->characterIndex = 0;
        s_findingFrameCount = 0;

        TestSystem::GetInstance()->SetMainThreadWorking(TRUE);
        TestSystem::GetInstance()->Yield();
    }

    void TestUtils::LeftClick(const String &label)
    {
        RPP_ASSERT(s_pData != nullptr);
        s_pData->label = label;
        s_pData->action = ImGuiItemAction::IMGUI_ACTION_CLICK;
        s_pData->text = "";
        s_pData->characterIndex = 0;
        s_findingFrameCount = 0;

        TestSystem::GetInstance()->SetMainThreadWorking(TRUE);
        TestSystem::GetInstance()->Yield();
    }

    void TestUtils::Type(const String &text)
    {
        RPP_ASSERT(s_pData != nullptr);
        s_pData->label = "";
        s_pData->action = ImGuiItemAction::IMGUI_ACTION_TYPE;
        s_pData->text = text;
        s_pData->characterIndex = 0;
        s_findingFrameCount = 0;

        TestSystem::GetInstance()->SetMainThreadWorking(TRUE);
        TestSystem::GetInstance()->Yield();
    }

    void TestUtils::Enter()
    {
        RPP_ASSERT(s_pData != nullptr);
        s_pData->label = "";
        s_pData->action = ImGuiItemAction::IMGUI_ACTION_TYPE;
        s_pData->text = "";
        s_pData->characterIndex = 0;
        s_findingFrameCount = 0;

        TestSystem::GetInstance()->SetMainThreadWorking(TRUE);
        TestSystem::GetInstance()->Yield();
    }

    b8 TestUtils::IsItemFound(const String &label)
    {
        RPP_ASSERT(s_pData != nullptr);
        s_pData->label = label;
        s_pData->action = ImGuiItemAction::IMGUI_ACTION_FIND_ITEM;
        s_findingFrameCount = 0;

        TestSystem::GetInstance()->Yield();

        while (s_findingFrameCount == 0)
        {
            TestSystem::GetInstance()->Yield();
        }

        b8 result = s_itemFound;
        ResetCurrentItem();
        return result;
    }

    void TestUtils::Assert(b8 condition, const String &message)
    {
        if (!condition)
        {
            REGISTER_ERROR("Assertion failed: {}", message);
            TestSystem::GetInstance()->Yield();
        }
    }

    void TestUtils::AssertInputTextValue(const String &label, const String &expectedValue)
    {
        RPP_ASSERT(s_pData != nullptr);
        s_pData->label = label;
        s_pData->action = ImGuiItemAction::IMGUI_ACTION_ASSERT_TEXT_INPUT;
        s_findingFrameCount = 0;

        s_pData->labelCheckingCallback = [expectedValue](const String &itemLabel, void *pData, ImGuiID id)
        {
            RPP_ASSERT(pData != nullptr);

            const char *inputTextLabel = static_cast<const char *>(pData);

            if (String(inputTextLabel) != expectedValue)
            {
                REGISTER_ERROR("InputText item with label '{}' has value '{}', expected '{}'",
                               itemLabel, String(inputTextLabel), expectedValue);
            }
        };

        TestSystem::GetInstance()->SetMainThreadWorking(TRUE);
        TestSystem::GetInstance()->Yield();
    }

    u32 TestUtils::FindRendererIdByName(const String &rendererName)
    {
#if defined(RPP_USE_TEST)
        Scope<Storage<Renderer::RendererData>> &renderers = Renderer::s_currentRenderers;
        u32 currentCapacity = renderers->GetCapacity();

        for (u32 rendererId = 0u; rendererId < currentCapacity; ++rendererId)
        {
            Renderer::RendererData *pData = renderers->Get(rendererId);

            if (pData == nullptr)
            {
                continue;
            }

            if (String(pData->window->GetWindowTitle()) == rendererName)
            {
                return rendererId;
            }
        }
#else
        RPP_UNUSED(rendererName);
#endif
        return INVALID_ID;
    }
} // namespace rpp
#endif