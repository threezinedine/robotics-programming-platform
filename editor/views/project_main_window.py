from PyQt6.QtWidgets import QMainWindow
from converted.project_main_window_ui import Ui_StartMainWindow
from components.dialogs import NewProjectDialog


class ProjectMainWindow(QMainWindow):
    """
    Contains the source tree for the current openning project.
    """

    def __init__(self) -> None:
        super().__init__()

        self.ui = Ui_StartMainWindow()
        self.ui.setupUi(self)  # type: ignore

        self.newProjectDialog = NewProjectDialog(self)

        self._SetupUI()

    def _SetupUI(self) -> None:
        self.ui.newProjectAction.triggered.connect(self._OnNewProject)

    def _OnNewProject(self) -> None:
        self.newProjectDialog.show()
