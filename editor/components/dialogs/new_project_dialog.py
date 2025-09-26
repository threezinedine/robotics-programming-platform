from typing import Optional
from PyQt6.QtWidgets import QDialog, QWidget
from converted_uis.new_project_dialog_ui import Ui_NewProjectDialog
from utils.dependency_injection import Depend


class NewProjectDialogViewModel:
    """
    The view model for the NewProjectDialog.
    """

    pass


@Depend(NewProjectDialogViewModel)
class NewProjectDialog(QDialog):
    """
    Used for making a new project.
    """

    def __init__(
        self,
        viewModel: NewProjectDialogViewModel,
        parent: Optional[QWidget] = None,
    ):
        super().__init__(parent)
        self.ui = Ui_NewProjectDialog()
        self.ui.setupUi(self)  # type: ignore

        self.viewModel = viewModel
