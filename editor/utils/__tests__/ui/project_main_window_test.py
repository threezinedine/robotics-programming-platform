import pytest  # type: ignore
from pytestqt.qtbot import QtBot
from views.project_main_window import ProjectMainWindow
from utils.dependency_injection import GetObject


def test_create_new_project(qtbot: QtBot, uiConfig: None) -> None:
    mainWindow = GetObject(ProjectMainWindow)

    qtbot.addWidget(mainWindow)
    mainWindow.show()

    mainWindow.ui.newProjectAction.trigger()

    mainWindow.newProjectDialog.ui.projectNameInput.setText("TestProject")
    mainWindow.newProjectDialog.ui.buttonBox.accepted.emit()

    assert mainWindow.windowTitle() == "Project - TestProject"
