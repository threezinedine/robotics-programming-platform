from packages import *  # this import will be deleted in the core, this line must be at the top of the file
from constants import *


def open_new_project_modal():
    ImGuiTestUtils.LeftClick(EDITOR_MENUBAR_FILE)
    ImGuiTestUtils.MoveToItem(EDITOR_MENUBAR_NEW)
    ImGuiTestUtils.LeftClick(EDITOR_MENUBAR_NEW_PROJECT)


def test_create_new_project():
    open_new_project_modal()

    ImGuiTestUtils.Assert(
        ImGuiTestUtils.IsItemFound(EDITOR_NEW_PROJECT_MODAL),
        "New Project modal not found",
    )

    ImGuiTestUtils.LeftClick(EDITOR_NEW_PROJECT_INPUT_PROJECT_NAME)
    ImGuiTestUtils.Type("test-project")

    ImGuiTestUtils.LeftClick(EDITOR_NEW_PROJECT_INPUT_PROJECT_FOLDER)
    ImGuiTestUtils.Type("/home/test")

    ImGuiTestUtils.LeftClick(EDITOR_NEW_PROJECT_CANCEL_BUTTON)

    TestSystem.Wait(10)

    ImGuiTestUtils.Assert(
        not ImGuiTestUtils.IsItemFound(EDITOR_NEW_PROJECT_MODAL),
        "New Project modal not closed after cancel",
    )

    open_new_project_modal()

    ImGuiTestUtils.AssertInputTextValue(EDITOR_NEW_PROJECT_INPUT_PROJECT_NAME, "")
    ImGuiTestUtils.AssertInputTextValue(EDITOR_NEW_PROJECT_INPUT_PROJECT_FOLDER, "")

    # Click create without filling fields -> should not close the modal
    ImGuiTestUtils.LeftClick(EDITOR_NEW_PROJECT_CREATE_BUTTON)
    TestSystem.Wait(10)
    ImGuiTestUtils.Assert(
        ImGuiTestUtils.IsItemFound(EDITOR_NEW_PROJECT_MODAL),
        "New Project modal closed despite missing fields",
    )
