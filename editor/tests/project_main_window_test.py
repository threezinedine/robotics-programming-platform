import os
import pytest  # type: ignore
from Engine import FromString_ProjectDescription
from pytestqt.qtbot import QtBot
from views.project_main_window import ProjectMainWindow
from utils.dependency_injection import GetObject
from pyfakefs.fake_filesystem import FakeFilesystem


def test_create_new_project(
    qtbot: QtBot,
    uiConfig: None,
    fs: FakeFilesystem,
) -> None:
    mainWindow = GetObject(ProjectMainWindow)

    qtbot.addWidget(mainWindow)
    mainWindow.show()

    mainWindow.ui.newProjectAction.trigger()

    mainWindow.newProjectDialog.ui.projectNameInput.setText("TestProject")
    mainWindow.newProjectDialog.ui.buttonBox.accepted.emit()

    assert mainWindow.windowTitle() == "Project - TestProject"

    folder = os.path.join(os.getcwd(), "TestProject")
    assert os.path.exists(folder) and os.path.isdir(folder)

    projectFile = os.path.join(folder, "init.rppproj")
    assert os.path.exists(projectFile) and os.path.isfile(projectFile)

    with open(projectFile, "r") as f:
        projectDesc = FromString_ProjectDescription(f.read())
        assert projectDesc.name == "TestProject"
