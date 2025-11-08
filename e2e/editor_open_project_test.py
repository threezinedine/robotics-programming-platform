import json
from packages import *  # this import will be deleted in the core, this line must be at the top of the file
from constants import *
from dataclasses import asdict
from context import context


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

    desc: ProjectDescription = ProjectDescription(name=testProjectName)

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

@context
def setup_recent_projects():
    editorData = EditorDataDescription(recentProjects=["/home/ok/project.rppproj", "/home/test/project.rppproj"])
    editorDataFilePath = "editor.json"
    file: FileHandle = FileSystem.OpenFile(editorDataFilePath, FILE_WRITE)
    FileSystem.Write(file, json.dumps(asdict(editorData), indent=4))
    FileSystem.CloseFile(file)

    projectData = ProjectDescription(name="test")
    file: FileHandle = FileSystem.OpenFile("/home/test/project.rppproj", FILE_WRITE)
    FileSystem.Write(file, json.dumps(asdict(projectData), indent=4))
    FileSystem.CloseFile(file)

    projectData = ProjectDescription(name="ok")
    file: FileHandle = FileSystem.OpenFile("/home/ok/project.rppproj", FILE_WRITE)
    FileSystem.Write(file, json.dumps(asdict(projectData), indent=4))
    FileSystem.CloseFile(file)

def test_open_project_from_recents(setup_recent_projects: None):
    TestUtils.LeftClick(EDITOR_MENUBAR_FILE)
    TestUtils.MoveToItem(EDITOR_MENUBAR_RECENTS)
    TestUtils.LeftClick(EDITOR_MENUBAR_RECENT_PROJECT_FORMAT.format(1))

    TestSystem.Wait(100)

    TestUtils.Assert(
        TestUtils.FindRendererIdByName("Editor - test") != INVALID_ID,
        "The opened project is not loaded"
    )

def test_auto_open_first_project_in_recents(setup_recent_projects: None):
    TestSystem.Wait(10)

    TestUtils.Assert(
        TestUtils.FindRendererIdByName("Editor - ok") != INVALID_ID,
        "The first project in recents was not auto-opened on editor start"
    )