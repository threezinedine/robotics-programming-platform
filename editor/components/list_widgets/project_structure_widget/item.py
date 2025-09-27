from PyQt6.QtWidgets import QTreeWidgetItem


class ProjectStructureItem(QTreeWidgetItem):
    def __init__(self, name: tuple[str, str]):
        finalName = name[0].replace(".rppfunc", "")  # TODO: refactor to more general
        super().__init__([finalName])
        self.holdValue = finalName
        self.folder = name[1]
