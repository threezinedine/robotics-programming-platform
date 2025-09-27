from Engine import Project
from utils.dependency_injection import Depend
from utils.signal import Signal


class ProjectState:
    """
    Singleton object which is used for managing how the current project is running.
    """

    CurrentProject: Project | None = None
    """
    The current openning project. Default to an empty project.
    """

    IsProjectLoaded: bool = False
    """
    Whether a project is currently loaded or not. False by default.
    """


@Depend(ProjectState)
class ProjectStateModel:
    def __init__(self, projectState: ProjectState) -> None:
        self.projectState = projectState
        self.projectDir = ""
        """
        The full path to the current project directory (include the folder with the name of the project).
        """
        self.currentProjectSignal = Signal()
        self.isProjectLoadedSignal = Signal()

        self.projectStructureChangedSignal = Signal()

    @property
    def CurrentProject(self) -> Project | None:
        return self.projectState.CurrentProject

    @CurrentProject.setter
    def CurrentProject(self, project: Project | None) -> None:
        self.projectState.CurrentProject = project
        self.currentProjectSignal.Emit(project)

    @property
    def IsProjectLoaded(self) -> bool:
        return self.projectState.IsProjectLoaded

    @IsProjectLoaded.setter
    def IsProjectLoaded(self, isLoaded: bool) -> None:
        self.projectState.IsProjectLoaded = isLoaded
        self.isProjectLoadedSignal.Emit(isLoaded)

    @property
    def CurrentProjectSignal(self) -> Signal:
        return self.currentProjectSignal

    @property
    def IsProjectLoadedSignal(self) -> Signal:
        return self.isProjectLoadedSignal
