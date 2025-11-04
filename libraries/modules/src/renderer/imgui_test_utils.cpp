#include "modules/renderer/imgui_test_utils.h"
#include "modules/input/input.h"
#include "modules/test_system/test_system.h"

namespace rpp
{
    ImGuiTestUtils::GlobalData *ImGuiTestUtils::s_pData = nullptr;
    ImGuiTestUtils::ItemData *ImGuiTestUtils::s_pCurrentItemData = nullptr;
    b8 ImGuiTestUtils::s_itemFound = FALSE;
    u8 ImGuiTestUtils::s_findingFrameCount = 0;
    u32 ImGuiTestUtils::s_focusedRendererId = INVALID_ID;

    void ImGuiTestUtils::Initialize()
    {
        RPP_ASSERT(s_pData == nullptr);
        RPP_ASSERT(s_pCurrentItemData == nullptr);
        s_pData = RPP_NEW(ImGuiTestUtils::GlobalData);
    }

    void ImGuiTestUtils::Shutdown()
    {
        RPP_ASSERT(s_pData != nullptr);
        RPP_DELETE(s_pData);
    }

    void ImGuiTestUtils::Update(f32 deltaTime)
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

#if 0
            b8 characterDone = InputSystem::PressChar(s_pData->text.CStr()[s_pData->characterIndex]);
            if (characterDone)
            {
                s_pData->characterIndex++;
                if (s_pData->characterIndex >= s_pData->text.Length())
                {
                    TestSystem::GetInstance()->SetMainThreadWorking(FALSE);
                    ResetCurrentItem();
                }
            }
#else
            for (i32 i = 0; i < s_pData->text.Length(); i++)
            {
                ImGuiIO &io = ImGui::GetIO();
                io.AddInputCharacter(static_cast<u32>(s_pData->text[i]));
            }

            TestSystem::GetInstance()->SetMainThreadWorking(FALSE);
            ResetCurrentItem();
#endif

            break;
        }
        default:
            RPP_UNREACHABLE();
        }
    }

    void ImGuiTestUtils::ResetCurrentItem()
    {
        s_pData->label = "";
        s_pData->action = ImGuiItemAction::IMGUI_ACTION_COUNT;
        s_itemFound = FALSE;

        if (s_pCurrentItemData != nullptr)
        {
            RPP_DELETE(s_pCurrentItemData);
            s_pCurrentItemData = nullptr;
        }
    }

    void ImGuiTestUtils::MoveToItem(const String &label)
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

    void ImGuiTestUtils::LeftClick(const String &label)
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

    void ImGuiTestUtils::Type(const String &text)
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

    b8 ImGuiTestUtils::IsItemFound(const String &label)
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
} // namespace rpp
