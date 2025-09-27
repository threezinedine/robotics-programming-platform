from PyQt6.QtWidgets import QMainWindow
from PyQt6.QtGui import QCloseEvent
from converted_uis.logic_window_ui import Ui_LogicWindow
from utils.logger import logger  # type: ignore
from utils.dependency_injection import DependencyInjection
from Engine import Renderer


class LogicWindow(QMainWindow):
    """
    The main window for the logic editor.
    """

    def __init__(self) -> None:
        super().__init__(DependencyInjection.GetObject("ProjectMainWindow"))
        self.ui = Ui_LogicWindow()
        self.ui.setupUi(self)  # type: ignore

        self._SetupUI()

    def _SetupUI(self) -> None:
        pass

    def closeEvent(self, a0: QCloseEvent) -> None:
        renderer = self.ui.logicOpenGLWidget.renderer
        if renderer >= 0:
            Renderer.DestroyRenderer(renderer)
        return super().closeEvent(a0)
