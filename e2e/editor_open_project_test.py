import json
from packages import *  # this import will be deleted in the core, this line must be at the top of the file
from constants import *
from dataclasses import asdict


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
