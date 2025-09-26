from PyQt6.QtWidgets import QMainWindow
from converted.project_main_window_ui import Ui_StartMainWindow
from components.dialogs import NewProjectDialog
from utils.dependency_injection import Depend, GetObject


class ProjectMainWindowViewModel:
    """
    The view model for the ProjectMainWindow.
    """

    pass


@Depend(ProjectMainWindowViewModel)
class ProjectMainWindow(QMainWindow):
    """
    Contains the source tree for the current openning project.
    """

    def __init__(self, viewModel: ProjectMainWindowViewModel) -> None:
        super().__init__()
        self.viewModel = viewModel

        self.ui = Ui_StartMainWindow()
        self.ui.setupUi(self)  # type: ignore

        self.newProjectDialog = GetObject(NewProjectDialog, parent=self)

        self._SetupUI()

    def _SetupUI(self) -> None:
        self.ui.newProjectAction.triggered.connect(self._OnNewProject)

    def _OnNewProject(self) -> None:
        self.newProjectDialog.show()
