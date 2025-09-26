from typing import Optional
from PyQt6.QtWidgets import QDialog, QWidget
from converted.new_project_dialog_ui import Ui_NewProjectDialog


class NewProjectDialog(QDialog):
    """
    Used for making a new project.
    """

    def __init__(self, parent: Optional[QWidget] = None):
        super().__init__(parent)
        self.ui = Ui_NewProjectDialog()
        self.ui.setupUi(self)  # type: ignore
