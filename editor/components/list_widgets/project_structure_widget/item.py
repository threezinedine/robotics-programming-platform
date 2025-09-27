from PyQt6.QtWidgets import QTreeWidgetItem


class ProjectStructureItem(QTreeWidgetItem):
    def __init__(self, name: str):
        super().__init__([name])
