from .dependency_injection import AsSingleton, AsTransient

from views.project_main_window import ProjectMainWindow, ProjectMainWindowViewModel
from components.dialogs.new_project_dialog import (
    NewProjectDialog,
    NewProjectDialogViewModel,
)
from models import ProjectState, ProjectStateModel


def StartupConfig() -> None:
    """
    Write all singleton/transient registrations here.
    """

    AsSingleton(ProjectState)
    AsSingleton(ProjectStateModel)

    AsSingleton(ProjectMainWindowViewModel)
    AsSingleton(NewProjectDialogViewModel)

    AsTransient(NewProjectDialog)
    AsTransient(ProjectMainWindow)
