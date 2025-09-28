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
    Renderer,
)
from utils.config import StartupConfig


def main():
    SingletonManager.Initialize()
    Logging.Setup(HandlerType.CONSOLE.value, LogLevel.DEBUG)
    StartupConfig()
    Renderer.Initialize()

    logger.info("Application started.")
    logger.info(f"Current Working Thread ID: {os.getpid()}")
    app = QApplication(sys.argv)
    qdarktheme.setup_theme()
    mainWindow = GetObject(ProjectMainWindow)
    mainWindow.show()
    app.exec()
    logger.info("Application exited.")

    Renderer.Shutdown()
    SingletonManager.Shutdown()


if __name__ == "__main__":
    main()
