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

    assert (
        mainWindow.viewModel.RecentProjects == []
    ), "There should be no recent projects."

    mainWindow.ui.newProjectAction.trigger()

    mainWindow.newProjectDialog.ui.projectNameInput.setText("TestProject")
    mainWindow.newProjectDialog.ui.buttonBox.accepted.emit()

    assert mainWindow.windowTitle() == "Project - TestProject"

    ApplicationAssert(recentProjects=["TestProject"]).Assert()

    ProjectDescriptionAssert(
        projectDir=os.getcwd(),
        projectName="TestProject",
    ).Assert()

    # ----- Recents project has been updated -----
    assert len(mainWindow.viewModel.RecentProjects) == 1


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


def test_only_five_recent_projects(
    qtbot: QtBot,
    uiConfig: None,
    fs: FakeFilesystem,
) -> None:
    EnvironmentBuilder().AddApplication(
        ApplicationBuilder()
        .AddRecentProject("Project5")
        .AddRecentProject("Project4")
        .AddRecentProject("Project3")
        .AddRecentProject("Project2")
        .AddRecentProject("Project1")
    ).AddProject(ProjectBuilder("Project1")).AddProject(
        ProjectBuilder("Project2")
    ).AddProject(
        ProjectBuilder("Project3")
    ).AddProject(
        ProjectBuilder("Project4")
    ).AddProject(
        ProjectBuilder("Project5")
    ).Build()

    mainWindow = GetObject(ProjectMainWindow)
    qtbot.addWidget(mainWindow)
    mainWindow.show()

    mainWindow.ui.newProjectAction.trigger()
    mainWindow.newProjectDialog.ui.projectNameInput.setText("Project6")
    mainWindow.newProjectDialog.ui.buttonBox.accepted.emit()

    ApplicationAssert(
        version="1.0.0",
        recentProjects=[
            "Project6",
            "Project5",
            "Project4",
            "Project3",
            "Project2",
        ],
    ).Assert()


def test_auto_load_recent_projects(
    qtbot: QtBot,
    uiConfig: None,
    fs: FakeFilesystem,
) -> None:
    EnvironmentBuilder().AddApplication(
        ApplicationBuilder().AddRecentProject("Project2").AddRecentProject("Project1")
    ).AddProject(ProjectBuilder("Project1")).AddProject(
        ProjectBuilder("Project2")
    ).Build()

    mainWindow = GetObject(ProjectMainWindow)
    qtbot.addWidget(mainWindow)
    mainWindow.show()

    assert mainWindow.windowTitle() == "Project - Project2"
    assert len(mainWindow.viewModel.RecentProjects) == 2
    assert len(mainWindow.ui.recentProjectsMenu.actions()) == 2


def test_open_project(
    qtbot: QtBot,
    uiConfig: None,
    fs: FakeFilesystem,
) -> None:
    EnvironmentBuilder().AddApplication(
        ApplicationBuilder().AddRecentProject("Project2").AddRecentProject("Project1")
    ).AddProject(ProjectBuilder("Project1")).AddProject(
        ProjectBuilder("Project2")
    ).Build()

    mainWindow = GetObject(ProjectMainWindow)
    qtbot.addWidget(mainWindow)
    mainWindow.show()

    mainWindow.ui.recentProjectsMenu.actions()[0].trigger()

    assert mainWindow.windowTitle() == "Project - Project2"
    assert len(mainWindow.viewModel.RecentProjects) == 2
    assert len(mainWindow.ui.recentProjectsMenu.actions()) == 2

    mainWindow.ui.recentProjectsMenu.actions()[1].trigger()
    assert mainWindow.windowTitle() == "Project - Project1"

    ApplicationAssert(
        version="1.0.0",
        recentProjects=[
            "Project1",
            "Project2",
        ],
    ).Assert()
