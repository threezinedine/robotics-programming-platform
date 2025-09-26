import os
from PyQt6.QtWidgets import QMainWindow
from PyQt6.QtGui import QAction
from converted_uis.project_main_window_ui import Ui_StartMainWindow
from components.dialogs import NewProjectDialog
from models import Application
from utils.dependency_injection import Depend, GetObject
from utils.logger import logger  # type: ignore
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

        self.newProjectDialog = GetObject(NewProjectDialog, parent=self)
        self.recentProjectsActions: list[QAction] = []

        self._SetupUI()

        # Setting up the application
        self.viewModel.LoadApplicationData()

    def _SetupUI(self) -> None:
        self.ui.newProjectAction.triggered.connect(self.newProjectDialog.show)
        self.viewModel.ProjectNameSignal.Attach(self._UpdateProjectName)
        self.viewModel.ProjectNameSignal.Emit(None)

        self.newProjectDialog.viewModel.SetOnConfirm(
            self.viewModel.OnConfirmNewProjectCallback
        )

        self.viewModel.applicationModel.RecentProjectsSignal.Attach(
            self._UpdateRecentProjectsMenu
        )

    def _UpdateProjectName(self, projectName: str) -> None:
        self.setWindowTitle(f"Project - {self.viewModel.ProjectName}")

    def _UpdateRecentProjectsMenu(self, application: Application) -> None:
        self.ui.recentProjectsMenu.clear()

        for recentProject in self.viewModel.RecentProjects:
            actionName = os.path.basename(recentProject)
            action = QAction(actionName, self)
            self.ui.recentProjectsMenu.addAction(action)
            self.recentProjectsActions.append(action)
