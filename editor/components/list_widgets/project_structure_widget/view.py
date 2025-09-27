from typing import Any
from PyQt6.QtGui import QMouseEvent
from PyQt6.QtWidgets import QTreeWidget, QWidget
from PyQt6.QtCore import Qt
from utils.logger import logger  # type: ignore
from utils.dependency_injection import GetObject
from .view_model import ProjectStructureWidgetViewModel
from models import ProjectStateModel
from .item import ProjectStructureItem


class ProjectStructureWidget(QTreeWidget):
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

        self.itemChanged.connect(self._ItemChanged)

    def _UpdateProjectStructure(self, _: Any) -> None:
        self.viewModel.ReloadProjectStructure()

        self.clear()

        for item in self.viewModel.projectStructure:
            if isinstance(item, tuple):
                treeItem = ProjectStructureItem(item)
                treeItem.setFlags(
                    treeItem.flags() | Qt.ItemFlag.ItemIsEditable
                )  # Make it editable
                self.addTopLevelItem(treeItem)

                if item[0] == self.projectState.newestItemName:
                    treeItem.setSelected(True)
                    self.editItem(treeItem)

    def _ItemChanged(
        self,
        item: ProjectStructureItem,
        column: int,
    ) -> None:
        self.viewModel.ChangeFunctionName(item.folder, item.text(0))

    def mouseDoubleClickEvent(self, e: QMouseEvent) -> None:
        item: ProjectStructureItem = self.itemAt(e.position().toPoint())  # type: ignore

        self.viewModel.openFunctionSignal.Emit(item.folder)
