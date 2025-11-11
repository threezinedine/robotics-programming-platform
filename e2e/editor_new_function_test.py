from packages import *  # this import will be deleted in the core, this line must be at the top of the file
import json
from setup import setup_recent_projects, setup_project_with_functions  # type: ignore
from constants import *
from helper import assert_current_editor_window_title


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

    file: FileHandle = FileSystem.OpenFile("/home/ok/project.rppproj", FILE_READ)
    content = FileSystem.Read(file)
    FileSystem.CloseFile(file)

    projDes = ProjectDescription(**json.loads(content))

    TestUtils.Assert(
        len(projDes.functionNames) == 0,
        "Project description function names is not empty",
    )

    TestUtils.LeftClick(EDITOR_MENUBAR_FILE)
    TestUtils.LeftClick(EDITOR_MENUBAR_SAVE)
    TestSystem.Wait(20)

    file = FileSystem.OpenFile("/home/ok/project.rppproj", FILE_READ)
    content = FileSystem.Read(file)
    FileSystem.CloseFile(file)

    projDes = ProjectDescription(**json.loads(content))

    TestUtils.Assert(
        projDes.functionNames.count("NewFunction") == 1,
        "Project description does not contain 'NewFunction' exactly once",
    )

    TestUtils.Assert(
        projDes.functionNames.count("NewFunction (Copy)") == 1,
        "Project description does not contain 'NewFunction (Copy)' exactly once",
    )


def test_close_window_while_unsaved_changes(setup_recent_projects: None):
    TestUtils.LeftClick(EDITOR_MAIN_TOOLBAR_NEW)
    TestUtils.LeftClick(EDITOR_MAIN_TOOLBAR_NEW_POPUP_FUNCTION)
    TestSystem.Wait(10)
    TestUtils.Enter()
    TestSystem.Wait(10)

    TestUtils.Assert(
        TestUtils.IsItemFound(EDITOR_MAIN_FUNCTION_FORMAT.format("NewFunction")),
        "New function does not exist after confirming name",
    )

    rendererId = TestUtils.FindRendererIdByName("Editor - ok*")
    TestUtils.CloseRendererById(rendererId)

    TestSystem.Wait(20)

    TestUtils.Assert(
        TestUtils.IsItemFound("Editor/UnsavedChangesModal"),
        "Unsaved changes modal not opened after trying to close window with unsaved changes",
    )

    TestUtils.LeftClick("Editor/UnsavedChangesModal/Cancel")

    TestSystem.Wait(20)

    TestUtils.Assert(
        not TestUtils.IsItemFound("Editor/UnsavedChangesModal"),
        "Unsaved changes modal not closed after canceling",
    )

    TestUtils.CloseRendererById(rendererId)
    TestSystem.Wait(20)
    TestUtils.LeftClick("Editor/UnsavedChangesModal/Save")

    TestSystem.Wait(20)

    file: FileHandle = FileSystem.OpenFile("/home/ok/project.rppproj", FILE_READ)
    content = FileSystem.Read(file)
    FileSystem.CloseFile(file)

    projDes = ProjectDescription(**json.loads(content))

    TestUtils.Assert(
        len(projDes.functionNames) == 1 and projDes.functionNames[0] == "NewFunction",
        "Project description function names does not contain only 'NewFunction' after saving from unsaved changes modal",
    )


def test_rename_function(setup_recent_projects: None):
    TestUtils.LeftClick(EDITOR_MAIN_TOOLBAR_NEW)
    TestUtils.LeftClick(EDITOR_MAIN_TOOLBAR_NEW_POPUP_FUNCTION)
    TestSystem.Wait(10)

    TestUtils.Type("FunctionToRename")
    TestUtils.Enter()
    TestSystem.Wait(10)
    TestUtils.Assert(
        TestUtils.IsItemFound(EDITOR_MAIN_FUNCTION_FORMAT.format("FunctionToRename")),
        "Function to rename does not exist after confirming name",
    )

    TestUtils.DoubleClick(EDITOR_MAIN_FUNCTION_FORMAT.format("FunctionToRename"))
    TestSystem.Wait(10)

    TestUtils.Assert(
        not TestUtils.IsItemFound(
            EDITOR_MAIN_FUNCTION_FORMAT.format("FunctionToRename")
        ),
        "Function to rename is not in renaming mode after double clicking on it",
    )

    TestUtils.Assert(
        TestUtils.IsItemFound(
            EDITOR_MAIN_FUNCTION_RENAME_FORMAT.format("FunctionToRename")
        ),
        "Function to rename rename input box not found after double clicking on it",
    )

    TestUtils.Type("RenamedFunction")

    TestUtils.Enter()
    TestSystem.Wait(10)

    TestUtils.Assert(
        TestUtils.IsItemFound(EDITOR_MAIN_FUNCTION_FORMAT.format("RenamedFunction")),
        "Renamed function does not exist after confirming new name",
    )


def test_open_context_menu_for_function(setup_project_with_functions: None):
    TestUtils.LeftClick(EDITOR_FILES)
    TestUtils.LeftClick(EDITOR_FILES)
    TestUtils.RightClick(EDITOR_MAIN_FUNCTION_FORMAT.format("ExistingFunction"))
    TestSystem.Wait(10)

    TestUtils.Assert(
        TestUtils.IsItemFound(
            EDITOR_MAIN_FUNCTION_CONTEXT_MENU_FORMAT.format("ExistingFunction")
        ),
        "Function context menu not found after right clicking on function",
    )

    TestUtils.LeftClick(
        EDITOR_MAIN_FUNCTION_CONTEXT_MENU_RENAME_FORMAT.format("ExistingFunction")
    )

    TestSystem.Wait(10)

    TestUtils.Assert(
        not TestUtils.IsItemFound(
            EDITOR_MAIN_FUNCTION_FORMAT.format("ExistingFunction")
        ),
        "Function is not in renaming mode after clicking rename in context menu",
    )

    TestUtils.Assert(
        TestUtils.IsItemFound(
            EDITOR_MAIN_FUNCTION_RENAME_FORMAT.format("ExistingFunction")
        ),
        "Function rename input box not found after clicking rename in context menu",
    )

    TestUtils.Type("RenamedExistingFunction")
    TestUtils.Enter()
    TestSystem.Wait(10)

    TestUtils.Assert(
        TestUtils.IsItemFound(
            EDITOR_MAIN_FUNCTION_FORMAT.format("RenamedExistingFunction")
        ),
        "Renamed function does not exist after confirming new name",
    )

    TestUtils.Assert(
        not TestUtils.IsItemFound(
            EDITOR_MAIN_FUNCTION_FORMAT.format("ExistingFunction")
        ),
        "Old function name still exists after renaming",
    )

    TestUtils.Assert(
        not TestUtils.IsItemFound(
            EDITOR_MAIN_FUNCTION_RENAME_FORMAT.format("ExistingFunction")
        ),
        "Function rename input box still exists after renaming",
    )

    TestUtils.Assert(
        not TestUtils.IsItemFound(
            EDITOR_MAIN_FUNCTION_RENAME_FORMAT.format("RenamedExistingFunction")
        ),
        "Function rename input box still exists after renaming",
    )


def test_delete_function(setup_project_with_functions: None):
    TestUtils.LeftClick(EDITOR_FILES)
    TestUtils.LeftClick(EDITOR_FILES)
    TestUtils.RightClick(EDITOR_MAIN_FUNCTION_FORMAT.format("ExistingFunction"))
    TestSystem.Wait(10)

    TestUtils.Assert(
        TestUtils.IsItemFound(
            EDITOR_MAIN_FUNCTION_CONTEXT_MENU_FORMAT.format("ExistingFunction")
        ),
        "Function context menu not found after right clicking on function",
    )

    TestUtils.LeftClick(
        EDITOR_MAIN_FUNCTION_CONTEXT_MENU_DELETE_FORMAT.format("ExistingFunction")
    )

    TestSystem.Wait(10)

    TestUtils.Assert(
        not TestUtils.IsItemFound(
            EDITOR_MAIN_FUNCTION_FORMAT.format("ExistingFunction")
        ),
        "Function still exists after deleting it from context menu",
    )

    assert_current_editor_window_title("Editor - ok*")
