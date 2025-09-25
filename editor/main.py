import sys
from PyQt6.QtWidgets import QApplication
from Engine import LogLevel, Logging, SingletonManager, HandlerType
from views.project_main_window import ProjectMainWindow
import qdarktheme  # type: ignore
from Engine import (
    LogLevel,
    Logging,
    SingletonManager,
    HandlerType,
    Project,
    ProjectDescription,
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
    SingletonManager.Shutdown()


if __name__ == "__main__":
    main()
