import sys
from PyQt6.QtWidgets import QApplication
from Engine import LogLevel, Logging, SingletonManager, HandlerType
from views.project_main_window import ProjectMainWindow
import qdarktheme  # type: ignore


def main():
    SingletonManager.Initialize()
    Logging.Setup(HandlerType.CONSOLE.value, LogLevel.DEBUG)

    app = QApplication(sys.argv)
    qdarktheme.setup_theme()
    mainWindow = ProjectMainWindow()
    mainWindow.show()
    app.exec()

    SingletonManager.Shutdown()


if __name__ == "__main__":
    main()
