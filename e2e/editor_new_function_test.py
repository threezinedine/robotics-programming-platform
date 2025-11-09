from packages import *  # this import will be deleted in the core, this line must be at the top of the file
from setup import setup_recent_projects  # type: ignore
from constants import *


def test_create_new_function(setup_recent_projects: None):
    TestUtils.LeftClick(EDITOR_MAIN_TOOLBAR_NEW)

    TestUtils.Assert(
        TestUtils.IsItemFound(EDITOR_MAIN_TOOLBAR_NEW_POPUP),
        "New popup not found after clicking Main Toolbar New button",
    )

    TestUtils.Assert(
        TestUtils.FindRendererIdByName("Editor - ok") != INVALID_ID,
        "Window name is not 'Editor - ok'",
    )

    TestUtils.LeftClick(EDITOR_MAIN_TOOLBAR_NEW_POPUP_FUNCTION)
    TestSystem.Wait(20)

    TestUtils.Assert(
        TestUtils.IsItemFound(EDITOR_MAIN_FUNCTION_RENAME_FORMAT.format("NewFunction")),
        "New function is not in editing mode after creation",
    )

    TestUtils.Assert(
        not TestUtils.IsItemFound(EDITOR_MAIN_FUNCTION_FORMAT.format("NewFunction")),
        "New function already exists before confirming name",
    )

    TestUtils.Enter()

    TestSystem.Wait(20)

    TestUtils.Assert(
        not TestUtils.IsItemFound(
            EDITOR_MAIN_FUNCTION_RENAME_FORMAT.format("NewFunction")
        ),
        "New function is still in editing mode after confirming name",
    )

    TestUtils.Assert(
        TestUtils.IsItemFound(EDITOR_MAIN_FUNCTION_FORMAT.format("NewFunction")),
        "New function does not exist after confirming name",
    )

    TestUtils.LeftClick(EDITOR_MAIN_TOOLBAR_NEW)
    TestSystem.Wait(10)
    TestUtils.LeftClick(EDITOR_MAIN_TOOLBAR_NEW_POPUP_FUNCTION)
    TestSystem.Wait(10)

    TestUtils.Enter()
    TestSystem.Wait(20)

    TestUtils.Assert(
        TestUtils.IsItemFound(EDITOR_MAIN_FUNCTION_FORMAT.format("NewFunction")),
        "New function editor not opened after creating new function",
    )

    TestUtils.Assert(
        TestUtils.IsItemFound(EDITOR_MAIN_FUNCTION_FORMAT.format("NewFunction (Copy)")),
        "New function editor not opened after creating new function",
    )

    TestUtils.Assert(
        TestUtils.FindRendererIdByName("Editor - ok*") != INVALID_ID,
        "Window name is not 'Editor - ok*'",
    )
