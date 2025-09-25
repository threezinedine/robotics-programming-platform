from PyQt6.QtWidgets import QMainWindow
from converted.project_main_window_ui import Ui_StartMainWindow


class ProjectMainWindow(QMainWindow):
    def __init__(self) -> None:
        super().__init__()

        self.ui = Ui_StartMainWindow()
        self.ui.setupUi(self)  # type: ignore
