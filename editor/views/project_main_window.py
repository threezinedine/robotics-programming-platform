import os
from utils.logger import logger  # type: ignore
from Engine import Project, ProjectDescription, ToString_ProjectDescription
from PyQt6.QtWidgets import QMainWindow
from converted_uis.project_main_window_ui import Ui_StartMainWindow
from components.dialogs import NewProjectDialog, NewProjectDialogViewModel
from utils.dependency_injection import Depend, GetObject
from models import ProjectStateModel
from utils.signal import Signal


@Depend(ProjectStateModel)
class ProjectMainWindowViewModel:
    """
    The view model for the ProjectMainWindow.
    """

    def __init__(self, projectStateModel: ProjectStateModel) -> None:
        self.projectStateModel = projectStateModel

        self.projectNameSignal = Signal()

    @property
    def ProjectName(self) -> str:
        if self.projectStateModel.CurrentProject is None:
            return "No Project"
        return self.projectStateModel.CurrentProject.GetName()

    @property
    def ProjectNameSignal(self) -> Signal:
        return self.projectNameSignal


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
        self.ui.newProjectAction.triggered.connect(self.newProjectDialog.show)
        self.viewModel.ProjectNameSignal.Attach(self._UpdateProjectName)
        self.viewModel.ProjectNameSignal.Emit(None)

        self.newProjectDialog.viewModel.SetOnConfirm(self._OnConfirmNewProjectCallback)

    def _UpdateProjectName(self, projectName: str) -> None:
        logger.debug(f"Project name updated to {self.viewModel.ProjectName}")
        self.setWindowTitle(f"Project - {self.viewModel.ProjectName}")

    def _OnConfirmNewProjectCallback(
        self,
        viewModel: NewProjectDialogViewModel,
    ) -> None:
        projectDesc = ProjectDescription()
        projectDesc.name = viewModel.ProjectName
        self.viewModel.projectStateModel.CurrentProject = Project.CreateProject(
            projectDesc
        )

        # ==== Create the project directory ====
        projectFolder = viewModel.ProjectPath
        projectName = viewModel.ProjectName

        fullPath = os.path.join(projectFolder, projectName)
        if not os.path.exists(fullPath):
            os.makedirs(fullPath)
            logger.info(f"Created project directory at {fullPath}")

        projectFile = os.path.join(fullPath, "init.rppproj")

        with open(projectFile, "w") as f:
            f.write(ToString_ProjectDescription(projectDesc))

        # ==== Update loaded state ====
        self.viewModel.projectStateModel.IsProjectLoaded = True

        self.viewModel.ProjectNameSignal.Emit(self.viewModel.ProjectName)
