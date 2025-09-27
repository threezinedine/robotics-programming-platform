import os
import sys
from PyQt6.QtWidgets import QApplication
import qdarktheme  # type: ignore
from utils.dependency_injection import GetObject
from utils.logger import logger
from views.project_main_window import ProjectMainWindow
from Engine import (
    LogLevel,
    Logging,
    SingletonManager,
    HandlerType,
)
from utils.config import StartupConfig


def main():
    SingletonManager.Initialize()
    Logging.Setup(HandlerType.CONSOLE.value, LogLevel.DEBUG)
    StartupConfig()

    logger.info("Application started.")
    logger.info(f"Current Working Thread ID: {os.getpid()}")
    app = QApplication(sys.argv)
    qdarktheme.setup_theme()
    mainWindow = GetObject(ProjectMainWindow)
    mainWindow.show()
    app.exec()
    logger.info("Application exited.")

    SingletonManager.Shutdown()


if __name__ == "__main__":
    main()
