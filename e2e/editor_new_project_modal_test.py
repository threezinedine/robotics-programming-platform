from packages import *  # this import will be deleted in the core, this line must be at the top of the file
from constants import *


def open_new_project_modal():
    TestUtils.LeftClick(EDITOR_MENUBAR_FILE)
    TestUtils.MoveToItem(EDITOR_MENUBAR_NEW)
    TestUtils.LeftClick(EDITOR_MENUBAR_NEW_PROJECT)


def test_create_new_project():
    # Open New Project modal and cancel
    open_new_project_modal()

    TestUtils.Assert(
        TestUtils.IsItemFound(EDITOR_NEW_PROJECT_MODAL),
        "New Project modal not found",
    )

    TestUtils.LeftClick(EDITOR_NEW_PROJECT_INPUT_PROJECT_NAME)
    TestUtils.Type("test-project")

    TestUtils.LeftClick(EDITOR_NEW_PROJECT_INPUT_PROJECT_FOLDER)
    TestUtils.Type("/home/test")

    TestUtils.LeftClick(EDITOR_NEW_PROJECT_CANCEL_BUTTON)

    TestSystem.Wait(10)

    TestUtils.Assert(
        not TestUtils.IsItemFound(EDITOR_NEW_PROJECT_MODAL),
        "New Project modal not closed after cancel",
    )

    # Re-open New Project modal to verify fields are reset

    open_new_project_modal()

    TestUtils.AssertInputTextValue(EDITOR_NEW_PROJECT_INPUT_PROJECT_NAME, "")
    TestUtils.AssertInputTextValue(EDITOR_NEW_PROJECT_INPUT_PROJECT_FOLDER, "")

    # Click create without filling fields -> should not close the modal
    TestUtils.LeftClick(EDITOR_NEW_PROJECT_CREATE_BUTTON)
    TestSystem.Wait(10)
    TestUtils.Assert(
        TestUtils.IsItemFound(EDITOR_NEW_PROJECT_MODAL),
        "New Project modal closed despite missing fields",
    )
