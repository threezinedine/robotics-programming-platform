import os
from utils.logger import logger  # type: ignore
from constants import PROJECT_FILE_NAME
from Engine import (
    Project,
    ProjectDescription,
    ToString_ProjectDescription,
    FromString_ProjectDescription,
)

from utils.dependency_injection import Depend, GetObject
from models import ProjectStateModel, ApplicationModel, FunctionModel
from utils.signal import Signal
from components.dialogs import NewProjectDialogViewModel


@Depend(ProjectStateModel, ApplicationModel)
class ProjectMainWindowViewModel:
    """
    The view model for the ProjectMainWindow.
    """

    def __init__(
        self,
        projectStateModel: ProjectStateModel,
        applicationModel: ApplicationModel,
    ) -> None:
        self.projectStateModel = projectStateModel
        self.applicationModel = applicationModel

        self.projectNameSignal = Signal()

    @property
    def ProjectName(self) -> str:
        if self.projectStateModel.CurrentProject is None:
            return "No Project"
        return self.projectStateModel.CurrentProject.GetName()

    @property
    def ProjectNameSignal(self) -> Signal:
        return self.projectNameSignal

    @property
    def RecentProjects(self) -> list[str]:
        return self.applicationModel.application.recentProjects

    def LoadProject(self, projectPath: str) -> None:
        """
        Open a project from the given path.

        Parameters
        ----------
        projectPath : str
            The absolute path to the project directory (e.g., C:/path/to/project).
        """
        projectFile = os.path.join(projectPath, PROJECT_FILE_NAME)
        with open(projectFile, "r") as f:
            projectDesc = FromString_ProjectDescription(f.read())
            self.projectStateModel.CurrentProject = Project.CreateProject(projectDesc)
            self.projectStateModel.IsProjectLoaded = True
            self.ProjectNameSignal.Emit(self.ProjectName)

        self.applicationModel.application.recentProjects.remove(projectPath)
        self.applicationModel.application.recentProjects.insert(0, projectPath)

        self.applicationModel.Save()

    def LoadApplicationData(self) -> None:
        self.applicationModel.Create()
        self.applicationModel.Load()

        if len(self.applicationModel.application.recentProjects) > 0:
            self.LoadProject(self.applicationModel.application.recentProjects[0])

        logger.debug(f"Recent projects: {self.RecentProjects} emitted")
        self.applicationModel.RecentProjectsSignal.Emit(
            self.applicationModel.application
        )

    def OnConfirmNewProjectCallback(
        self,
        viewModel: NewProjectDialogViewModel,
    ) -> None:
        """
        Be called when a new project is confirmed in the NewProjectDialog.
        """

        projectDesc = ProjectDescription()
        projectDesc.name = viewModel.ProjectName
        self.projectStateModel.CurrentProject = Project.CreateProject(projectDesc)

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
        self.projectStateModel.IsProjectLoaded = True

        self.ProjectNameSignal.Emit(self.ProjectName)

        # ==== Update recent projects ====
        self.applicationModel.application.recentProjects.insert(0, fullPath)
        self.applicationModel.application.recentProjects = (
            self.applicationModel.application.recentProjects[:5]
        )
        self.applicationModel.Save()
        self.applicationModel.RecentProjectsSignal.Emit(
            self.applicationModel.application
        )

    def OpenProject(self, projectPath: str) -> None:
        """
        Open a project from the given path.

        Parameters
        ----------
        projectPath : str
            The absolute path to the project directory (e.g., C:/path/to/project).
        """
        self.LoadProject(projectPath)

    def CreateNewFunction(self) -> None:
        """
        Trigger this method to create a new function in the current project.
        """
        functionModel = GetObject(FunctionModel)
        functionModel.CreateNewFunction("NewFunction")
        functionModel.Save()
