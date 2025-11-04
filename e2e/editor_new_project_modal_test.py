from packages import *  # this import will be deleted in the core, this line must be at the top of the file
from constants import *


def open_new_project_modal():
    ImGuiTestUtils.LeftClick(EDITOR_MENUBAR_FILE_LABEL)
    ImGuiTestUtils.MoveToItem(EDITOR_MENUBAR_NEW_LABEL)
    ImGuiTestUtils.LeftClick(EDITOR_MENUBAR_NEW_PROJECT_LABEL)


def test_create_new_project():
    open_new_project_modal()

    assert ImGuiTestUtils.IsItemFound(
        EDITOR_NEW_PROJECT_MODAL_LABEL
    ), "New Project modal not found"

    ImGuiTestUtils.LeftClick(EDITOR_NEW_PROJECT_INPUT_PROJECT_NAME_LABEL)
    ImGuiTestUtils.Type("test-project")

    ImGuiTestUtils.LeftClick(EDITOR_NEW_PROJECT_INPUT_PROJECT_FOLDER_LABEL)
    ImGuiTestUtils.Type("/home/test")

    ImGuiTestUtils.LeftClick(EDITOR_NEW_PROJECT_CANCEL_BUTTON_LABEL)

    assert not ImGuiTestUtils.IsItemFound(
        EDITOR_NEW_PROJECT_MODAL_LABEL
    ), "New Project modal not closed after cancel"

    open_new_project_modal()

    ImGuiTestUtils.AssertInputTextValue(EDITOR_NEW_PROJECT_INPUT_PROJECT_NAME_LABEL, "")
    ImGuiTestUtils.AssertInputTextValue(
        EDITOR_NEW_PROJECT_INPUT_PROJECT_FOLDER_LABEL, ""
    )
