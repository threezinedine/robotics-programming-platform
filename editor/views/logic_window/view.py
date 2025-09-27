from PyQt6.QtWidgets import QMainWindow
from converted_uis.logic_window_ui import Ui_LogicWindow
from utils.dependency_injection import DependencyInjection


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
