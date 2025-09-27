from .dependency_injection import AsSingleton, AsTransient

from views.project_main_window import ProjectMainWindow, ProjectMainWindowViewModel
from views.logic_window import LogicWindow
from components.dialogs import (
    NewProjectDialog,
    NewProjectDialogViewModel,
)
from components.list_widgets import (
    ProjectStructureWidget,
    ProjectStructureWidgetViewModel,
)
from models import (
    ProjectState,
    ProjectStateModel,
    Application,
    ApplicationModel,
    FunctionModel,
)


def StartupConfig() -> None:
    """
    Write all singleton/transient registrations here.
    """

    AsTransient(FunctionModel)

    AsSingleton(Application)
    AsSingleton(ApplicationModel)

    AsSingleton(ProjectStructureWidget)
    AsSingleton(ProjectStructureWidgetViewModel)

    AsSingleton(ProjectState)
    AsSingleton(ProjectStateModel)

    AsTransient(LogicWindow)

    AsSingleton(ProjectMainWindowViewModel)
    AsSingleton(NewProjectDialogViewModel)

    AsTransient(NewProjectDialog)
    AsTransient(ProjectMainWindow)
