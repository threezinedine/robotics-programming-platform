from .dependency_injection import AsSingleton, AsTransient

from views.project_main_window import ProjectMainWindow, ProjectMainWindowViewModel
from components.dialogs.new_project_dialog import (
    NewProjectDialog,
    NewProjectDialogViewModel,
)


def StartupConfig() -> None:
    """
    Write all singleton/transient registrations here.
    """

    AsSingleton(ProjectMainWindowViewModel)
    AsSingleton(NewProjectDialogViewModel)

    AsTransient(NewProjectDialog)
    AsTransient(ProjectMainWindow)
