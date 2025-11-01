#include "modules/renderer/imgui_test_utils.h"
#include "modules/input/input.h"
#include "modules/test_system/test_system.h"

namespace rpp
{
    ImGuiTestUtils::GlobalData *ImGuiTestUtils::s_pData = nullptr;
    ImGuiTestUtils::ItemData *ImGuiTestUtils::s_pCurrentItemData = nullptr;

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

        if (s_pData->label == "" || s_pData->action == ImGuiItemAction::IMGUI_ACTION_COUNT)
        {
            return;
        }

        switch (s_pData->action)
        {
        case ImGuiItemAction::IMGUI_ACTION_MOVE:
        {
            if (s_pCurrentItemData != nullptr)
            {
                b8 arrived = InputSystem::MoveMouseTo(s_pCurrentItemData->position.x, s_pCurrentItemData->position.y);
                if (arrived)
                {
                    TestSystem::GetInstance()->SetMainThreadWorking(FALSE);
                    ResetCurrentItem();
                }
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

        TestSystem::GetInstance()->SetMainThreadWorking(TRUE);
        TestSystem::GetInstance()->Yield();
    }
} // namespace rpp
