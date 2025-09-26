import os
import pytest  # type: ignore
from pytestqt.qtbot import QtBot
from views.project_main_window import ProjectMainWindow
from utils.dependency_injection import GetObject
from pyfakefs.fake_filesystem import FakeFilesystem
from tests.utils import ProjectDescriptionAssert, ApplicationAssert
from tests.utils import EnvironmentBuilder, ApplicationBuilder, ProjectBuilder


def test_create_new_project(
    qtbot: QtBot,
    uiConfig: None,
    fs: FakeFilesystem,
) -> None:
    EnvironmentBuilder().EmptyApplication().Build()

    mainWindow = GetObject(ProjectMainWindow)

    qtbot.addWidget(mainWindow)
    mainWindow.show()

    mainWindow.ui.newProjectAction.trigger()

    mainWindow.newProjectDialog.ui.projectNameInput.setText("TestProject")
    mainWindow.newProjectDialog.ui.buttonBox.accepted.emit()

    assert mainWindow.windowTitle() == "Project - TestProject"

    ApplicationAssert(recentProjects=["TestProject"]).Assert()

    ProjectDescriptionAssert(
        projectDir=os.getcwd(),
        projectName="TestProject",
    ).Assert()


def test_existed_application(
    qtbot: QtBot,
    uiConfig: None,
    fs: FakeFilesystem,
) -> None:
    EnvironmentBuilder().AddApplication(
        ApplicationBuilder().SetVersion(1, 0, 1)
    ).Build()

    mainWindow = GetObject(ProjectMainWindow)
    qtbot.addWidget(mainWindow)
    mainWindow.show()

    ApplicationAssert(version="1.0.1").Assert()


def test_auto_open_recent_project(
    qtbot: QtBot,
    uiConfig: None,
    fs: FakeFilesystem,
) -> None:
    EnvironmentBuilder().AddApplication(
        ApplicationBuilder().AddRecentProject("RecentProject")
    ).AddProject(ProjectBuilder("RecentProject")).Build()

    mainWindow = GetObject(ProjectMainWindow)
    qtbot.addWidget(mainWindow)
    mainWindow.show()

    assert mainWindow.windowTitle() == "Project - RecentProject"

    ApplicationAssert(version="1.0.0", recentProjects=["RecentProject"]).Assert()

    ProjectDescriptionAssert(
        projectDir=os.getcwd(),
        projectName="RecentProject",
    ).Assert()
