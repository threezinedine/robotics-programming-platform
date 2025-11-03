#include "modules/renderer/imgui_test_utils.h"
#include "modules/input/input.h"
#include "modules/test_system/test_system.h"

namespace rpp
{
    ImGuiTestUtils::GlobalData *ImGuiTestUtils::s_pData = nullptr;
    ImGuiTestUtils::ItemData *ImGuiTestUtils::s_pCurrentItemData = nullptr;
    b8 ImGuiTestUtils::s_itemFound = FALSE;
    u8 ImGuiTestUtils::s_findingFrameCount = 0;

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
                RPP_LOG_ERROR("Cannot find ImGui item with label '{}'", s_pData->label);
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

                if (done)
                {
                    TestSystem::GetInstance()->SetMainThreadWorking(FALSE);
                    ResetCurrentItem();
                }
            }
            else
            {
                RPP_LOG_ERROR("Cannot find ImGui item with label '{}'", s_pData->label);
                TestSystem::GetInstance()->SetMainThreadWorking(FALSE);
                ResetCurrentItem();
            }
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
        s_findingFrameCount = 0;

        TestSystem::GetInstance()->SetMainThreadWorking(TRUE);
        TestSystem::GetInstance()->Yield();
    }

    void ImGuiTestUtils::LeftClick(const String &label)
    {
        RPP_ASSERT(s_pData != nullptr);
        s_pData->label = label;
        s_pData->action = ImGuiItemAction::IMGUI_ACTION_CLICK;
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
