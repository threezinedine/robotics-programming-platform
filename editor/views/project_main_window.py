import os
import json
from utils.logger import logger  # type: ignore
from constants import PROJECT_FILE_NAME, APPLICATION_FOLDER_NAME, APPLICATION_FILE_NAME
from Engine import Project, ProjectDescription, ToString_ProjectDescription
from PyQt6.QtWidgets import QMainWindow
from converted_uis.project_main_window_ui import Ui_StartMainWindow
from components.dialogs import NewProjectDialog, NewProjectDialogViewModel
from utils.dependency_injection import Depend, GetObject
from models import ProjectStateModel, Application
from dataclasses import asdict
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

        # Setting up the application
        self._LoadApplicationData()

        self.newProjectDialog = GetObject(NewProjectDialog, parent=self)

        self._SetupUI()

    def _SetupUI(self) -> None:
        self.ui.newProjectAction.triggered.connect(self.newProjectDialog.show)
        self.viewModel.ProjectNameSignal.Attach(self._UpdateProjectName)
        self.viewModel.ProjectNameSignal.Emit(None)

        self.newProjectDialog.viewModel.SetOnConfirm(self._OnConfirmNewProjectCallback)

    def _LoadApplicationData(self) -> None:
        logger.debug("Loading application data...")
        appdataFolder = os.getenv("APPDATA")
        assert appdataFolder is not None, "APPDATA environment variable is not set."
        appFolder = os.path.join(appdataFolder, APPLICATION_FOLDER_NAME)
        if not os.path.exists(appFolder):
            os.makedirs(appFolder)
            logger.info(f"Created application folder at {appFolder}")

        appFile = os.path.join(appFolder, APPLICATION_FILE_NAME)
        if not os.path.exists(appFile):
            with open(appFile, "w") as f:
                f.write(json.dumps(asdict(Application()), indent=4))

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

        projectFile = os.path.join(fullPath, PROJECT_FILE_NAME)

        with open(projectFile, "w") as f:
            logger.debug(f"Creating project file at {projectFile}")
            f.write(ToString_ProjectDescription(projectDesc))

        # ==== Update loaded state ====
        self.viewModel.projectStateModel.IsProjectLoaded = True

        self.viewModel.ProjectNameSignal.Emit(self.viewModel.ProjectName)
