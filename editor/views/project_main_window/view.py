from PyQt6.QtWidgets import QMainWindow
from converted_uis.project_main_window_ui import Ui_StartMainWindow
from components.dialogs import NewProjectDialog
from utils.dependency_injection import Depend, GetObject
from .view_model import ProjectMainWindowViewModel


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

        # Setting up the application
        self.viewModel.LoadApplicationData()

        self.newProjectDialog = GetObject(NewProjectDialog, parent=self)

        self._SetupUI()

    def _SetupUI(self) -> None:
        self.ui.newProjectAction.triggered.connect(self.newProjectDialog.show)
        self.viewModel.ProjectNameSignal.Attach(self._UpdateProjectName)
        self.viewModel.ProjectNameSignal.Emit(None)

        self.newProjectDialog.viewModel.SetOnConfirm(
            self.viewModel.OnConfirmNewProjectCallback
        )

    def _UpdateProjectName(self, projectName: str) -> None:
        self.setWindowTitle(f"Project - {self.viewModel.ProjectName}")
