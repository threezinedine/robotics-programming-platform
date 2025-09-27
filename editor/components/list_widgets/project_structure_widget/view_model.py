from utils.dependency_injection import Depend
from models import ProjectStateModel


@Depend(ProjectStateModel)
class ProjectStructureWidgetViewModel:
    def __init__(self, projectState: ProjectStateModel) -> None:
        self.projectState = projectState
