from packages import *  # this import will be deleted in the core, this line must be at the top of the file
import json
from context import context


@context
def setup_recent_projects():
    editorData = EditorDataDescription(
        recentProjects=["/home/ok/project.rppproj", "/home/test/project.rppproj"]
    )
    editorDataFilePath = "editor.json"
    file: FileHandle = FileSystem.OpenFile(editorDataFilePath, FILE_WRITE)
    FileSystem.Write(file, json.dumps(asdict(editorData), indent=4))
    FileSystem.CloseFile(file)

    projectData = ProjectDescription(name="test", functionNames=[])
    file: FileHandle = FileSystem.OpenFile("/home/test/project.rppproj", FILE_WRITE)
    FileSystem.Write(file, json.dumps(asdict(projectData), indent=4))
    FileSystem.CloseFile(file)

    projectData = ProjectDescription(name="ok", functionNames=[])
    file: FileHandle = FileSystem.OpenFile("/home/ok/project.rppproj", FILE_WRITE)
    FileSystem.Write(file, json.dumps(asdict(projectData), indent=4))
    FileSystem.CloseFile(file)


@context
def setup_project_with_functions(setup_recent_projects: None):
    projectData = ProjectDescription(name="ok", functionNames=["ExistingFunction"])
    file: FileHandle = FileSystem.OpenFile("/home/ok/project.rppproj", FILE_WRITE)
    FileSystem.Write(file, json.dumps(asdict(projectData), indent=4))
    FileSystem.CloseFile(file)
