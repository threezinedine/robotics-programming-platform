from typing import Any
from PyQt6.QtWidgets import QListWidget, QWidget
from utils.logger import logger  # type: ignore
from utils.dependency_injection import GetObject
from .view_model import ProjectStructureWidgetViewModel
from models import ProjectStateModel


class ProjectStructureWidget(QListWidget):
    """
    The override of the default QListWidget to show the project structure.
    """

    def __init__(
        self,
        parent: QWidget,
    ) -> None:
        super().__init__(parent)
        self.viewModel = GetObject(ProjectStructureWidgetViewModel)
        self.projectState = GetObject(ProjectStateModel)

        self._SetupUI()

    def _SetupUI(self) -> None:
        logger.debug("Setting up the ProjectStructureWidget UI.")
        self.projectState.projectStructureChangedSignal.Attach(
            self._UpdateProjectStructure
        )

    def _UpdateProjectStructure(self, _: Any) -> None:
        logger.debug("Updating the project structure view.")
        self.viewModel.ReloadProjectStructure()
        logger.debug(f"New project structure: {self.viewModel.projectStructure}")

        self.clear()

        for item in self.viewModel.projectStructure:
            if isinstance(item, str) and item.endswith(".rppfunc"):
                self.addItem(item)
