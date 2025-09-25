import sys
from PyQt6.QtWidgets import QApplication
import qdarktheme  # type: ignore
from views.project_main_window import ProjectMainWindow
from Engine import (
    LogLevel,
    Logging,
    SingletonManager,
    HandlerType,
    Project,
    ProjectDescription,
    ToString_ProjectDescription,
)


def main():
    SingletonManager.Initialize()
    Logging.Setup(HandlerType.CONSOLE.value, LogLevel.DEBUG)

    app = QApplication(sys.argv)
    qdarktheme.setup_theme()
    mainWindow = ProjectMainWindow()
    mainWindow.show()
    app.exec()

    desc = ProjectDescription()
    desc.name = "TestProject"
    project = Project.CreateProject(desc)
    Logging.Log(LogLevel.ERROR, f"Project: {project.GetName()}", __file__, 10)
    projectDest: ProjectDescription = ProjectDescription()
    projectDest.name = "TestProject"
    project = Project.CreateProject(projectDest)
    Logging.Log(
        LogLevel.ERROR,
        f"Project: {project.GetName()} - Json: {ToString_ProjectDescription(projectDest)}",
        __file__,
        10,
    )
    SingletonManager.Shutdown()


if __name__ == "__main__":
    main()
