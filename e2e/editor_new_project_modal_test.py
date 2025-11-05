from packages import *  # this import will be deleted in the core, this line must be at the top of the file
import os
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
    TestUtils.MoveToItem(EDITOR_NEW_PROJECT_CREATE_BUTTON)
    TestUtils.LeftClick(EDITOR_NEW_PROJECT_CREATE_BUTTON)
    TestSystem.Wait(10)
    TestUtils.Assert(
        TestUtils.IsItemFound(EDITOR_NEW_PROJECT_MODAL),
        "New Project modal closed despite missing fields",
    )

    TestUtils.Assert(
        TestUtils.IsItemFound(EDITOR_NEW_PROJECT_ERROR_MESSAGE),
        "Error message not shown for missing fields",
    )

    # Fill field project folder with non-existing path -> should not close the modal
    TestUtils.LeftClick(EDITOR_NEW_PROJECT_CANCEL_BUTTON)
    TestSystem.Wait(10)
    open_new_project_modal()

    TestUtils.Assert(
        not TestUtils.IsItemFound(EDITOR_NEW_PROJECT_ERROR_MESSAGE),
        "Error message still shown after modal closed",
    )

    testFolder = "/home/test-folder"

    TestUtils.Assert(
        not FileSystem.PathExists(testFolder),
        "Test folder already exists, cannot proceed with the test",
    )

    TestUtils.LeftClick(EDITOR_NEW_PROJECT_INPUT_PROJECT_NAME)
    TestUtils.Type("test-project")

    TestUtils.LeftClick(EDITOR_NEW_PROJECT_INPUT_PROJECT_FOLDER)
    TestUtils.Type(testFolder)

    TestUtils.LeftClick(EDITOR_NEW_PROJECT_CREATE_BUTTON)
    TestSystem.Wait(10)

    TestUtils.Assert(
        TestUtils.IsItemFound(EDITOR_NEW_PROJECT_ERROR_MESSAGE),
        "Error message not shown for missing fields",
    )

    TestUtils.Assert(
        TestUtils.IsItemFound(EDITOR_NEW_PROJECT_MODAL),
        "New Project modal closed despite non-existing project folder",
    )

    # Fill fields with valid project folder and existed project name (the folder inside the project folder) ->
    # should not able to create the project
    testMatchProjectName = "test-project"
    FileSystem.CreateDirectory(testFolder)
    FileSystem.CreateDirectory(os.path.join(testFolder, testMatchProjectName))

    TestUtils.LeftClick(EDITOR_NEW_PROJECT_CANCEL_BUTTON)
    TestSystem.Wait(100)

    open_new_project_modal()

    TestUtils.Assert(
        not TestUtils.IsItemFound(EDITOR_NEW_PROJECT_ERROR_MESSAGE),
        "Error message still shown after modal closed",
    )

    TestUtils.LeftClick(EDITOR_NEW_PROJECT_INPUT_PROJECT_NAME)
    TestUtils.Type(testMatchProjectName)

    TestUtils.LeftClick(EDITOR_NEW_PROJECT_INPUT_PROJECT_FOLDER)
    TestUtils.Type(testFolder)

    TestUtils.LeftClick(EDITOR_NEW_PROJECT_CREATE_BUTTON)
    TestSystem.Wait(10)

    TestUtils.Assert(
        TestUtils.IsItemFound(EDITOR_NEW_PROJECT_MODAL),
        "New Project modal closed despite existing project with the same name",
    )

    TestUtils.LeftClick(EDITOR_NEW_PROJECT_CANCEL_BUTTON)
    TestSystem.Wait(10)
    open_new_project_modal()

    newProjectName = "new-test-project"
    # Fill fields with valid project folder and non-existed project name -> should create the project and close the modal
    TestUtils.LeftClick(EDITOR_NEW_PROJECT_INPUT_PROJECT_NAME)
    TestUtils.Type(newProjectName)
    TestUtils.LeftClick(EDITOR_NEW_PROJECT_INPUT_PROJECT_FOLDER)
    TestUtils.Type(testFolder)

    TestUtils.LeftClick(EDITOR_NEW_PROJECT_CREATE_BUTTON)
    TestSystem.Wait(10)

    TestUtils.Assert(
        not TestUtils.IsItemFound(EDITOR_NEW_PROJECT_MODAL),
        "New Project modal not closed after creating a valid project",
    )


def test_new_project_effect():
    open_new_project_modal()
    projectFolder = "/home/test"
    projectName = "test"

    FileSystem.CreateDirectory(projectFolder)

    TestUtils.MoveToItem(EDITOR_NEW_PROJECT_INPUT_PROJECT_NAME)
    TestUtils.LeftClick(EDITOR_NEW_PROJECT_INPUT_PROJECT_NAME)
    TestUtils.Type(projectName)
    TestUtils.LeftClick(EDITOR_NEW_PROJECT_INPUT_PROJECT_FOLDER)
    TestUtils.Type(projectFolder)

    TestUtils.Assert(
        not FileSystem.PathExists(os.path.join(projectFolder, projectName)),
        "Project folder already exists on filesystem before creating the project",
    )

    TestUtils.Assert(
        TestUtils.FindRendererIdByName("Editor") != INVALID_ID,
        f"The current window is not 'Editor'",
    )

    TestUtils.LeftClick(EDITOR_NEW_PROJECT_CREATE_BUTTON)
    TestSystem.Wait(10)

    TestUtils.Assert(
        FileSystem.PathExists(os.path.join(projectFolder, projectName)),
        "Project folder not created on filesystem",
    )

    TestUtils.Assert(
        FileSystem.PathExists(
            os.path.join(projectFolder, projectName, "project.rppproj")
        ),
        "The project file not created on filesystem",
    )

    TestUtils.Assert(
        not FileSystem.IsDirectory(
            os.path.join(projectFolder, projectName, "project.rppproj")
        ),
        "The project file is created as a directory instead of a file",
    )

    TestUtils.Assert(
        TestUtils.FindRendererIdByName("Editor") == INVALID_ID,
        "The current window is still 'Editor'",
    )

    TestUtils.Assert(
        TestUtils.FindRendererIdByName("Editor - test") != INVALID_ID,
        "The current window is not 'Editor - test'",
    )
