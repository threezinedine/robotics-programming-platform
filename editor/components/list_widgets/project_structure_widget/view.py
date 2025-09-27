from PyQt6.QtWidgets import QListWidget, QWidget
from utils.logger import logger
from utils.dependency_injection import GetObject
from .view_model import ProjectStructureWidgetViewModel


class ProjectStructureWidget(QListWidget):
    """
    The override of the default QListWidget to show the project structure.
    """

    def __init__(self, parent: QWidget) -> None:
        super().__init__(parent)
        self.viewModel = GetObject(ProjectStructureWidgetViewModel)

        logger.debug("ProjectStructureWidget initialized.")
