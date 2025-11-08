from packages import *  # this import will be deleted in the core, this line must be at the top of the file
import json
from constants import *
from dataclasses import asdict
from setup import setup_recent_projects #type: ignore


def test_open_project():
    TestUtils.LeftClick(EDITOR_MENUBAR_FILE)
    TestUtils.MoveToItem(EDITOR_MENUBAR_OPEN)
    TestUtils.LeftClick(EDITOR_MENUBAR_OPEN_PROJECT)

    TestSystem.Wait(10)

    TestUtils.Assert(
        TestUtils.IsItemFound(EDITOR_OPEN_PROJECT_MODAL),
        "Open Project modal not found after clicking File->Open->Project",
    )

    # create project path
    testProjectPath = "/home/test"
    testProjectName = "TestProject"
    finalProjectPath = f"{testProjectPath}/{testProjectName}/project.rpproj"

    FileSystem.CreateDirectory(testProjectPath)
    FileSystem.CreateDirectory(f"{testProjectPath}/{testProjectName}")

    desc: ProjectDescription = ProjectDescription(name=testProjectName, functionNames=[])

    file: FileHandle = FileSystem.OpenFile(finalProjectPath, FILE_WRITE)
    FileSystem.Write(file, json.dumps(asdict(desc), indent=4))
    FileSystem.CloseFile(file)

    TestUtils.LeftClick(EDITOR_OPEN_PROJECT_MODAL_PATH_INPUT)
    TestUtils.Type(finalProjectPath)
    TestUtils.LeftClick(EDITOR_OPEN_PROJECT_OK_BUTTON)
    TestSystem.Wait(20)

    TestUtils.Assert(
        TestUtils.FindRendererIdByName(f"Editor - {testProjectName}") != INVALID_ID,
        "Project was not opened successfully",
    )

def test_open_project_from_recents(setup_recent_projects: None):
    TestUtils.LeftClick(EDITOR_MENUBAR_FILE)
    TestUtils.MoveToItem(EDITOR_MENUBAR_RECENTS)
    TestUtils.LeftClick(EDITOR_MENUBAR_RECENT_PROJECT_FORMAT.format(1))

    TestSystem.Wait(100)

    TestUtils.Assert(
        TestUtils.FindRendererIdByName("Editor - test") != INVALID_ID,
        "The opened project is not loaded"
    )

    # the current project should be at the top of recents now
    file: FileHandle = FileSystem.OpenFile("editor.json", FILE_READ)
    fileContent = FileSystem.Read(file)
    editorData = EditorDataDescription(**json.loads(fileContent))
    FileSystem.CloseFile(file)

    TestUtils.Assert(
        editorData.recentProjects[0] == "/home/test/project.rppproj",
        "The opened project is not at the top of recents list"
    )


def test_auto_open_first_project_in_recents(setup_recent_projects: None):
    TestSystem.Wait(10)

    TestUtils.Assert(
        TestUtils.FindRendererIdByName("Editor - ok") != INVALID_ID,
        "The first project in recents was not auto-opened on editor start"
    )