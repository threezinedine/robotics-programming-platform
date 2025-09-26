import os
from typing import Optional, Callable, TypeAlias
from PyQt6.QtWidgets import QDialog, QWidget, QFileDialog
from converted_uis.new_project_dialog_ui import Ui_NewProjectDialog
from utils.dependency_injection import Depend
from utils.signal import Signal

ConfirmCallback: TypeAlias = Callable[["NewProjectDialogViewModel"], None]


class NewProjectDialogViewModel:
    """
    The view model for the NewProjectDialog.
    """

    def __init__(self) -> None:
        self._onConfirm: ConfirmCallback | None = None
        self._projectName: str = ""
        self._projectNameSignal: Signal = Signal()
        self._projectNameSignal.Attach(self._UpdateProjectName)

        self._projectPath: str = ""
        self._projectPathSignal: Signal = Signal()
        self._projectPathSignal.Attach(self._UpdateProjectPath)

    def _UpdateProjectName(self, name: str) -> None:
        self._projectName = name

    def _UpdateProjectPath(self, path: str) -> None:
        self._projectPath = path

    @property
    def ProjectName(self) -> str:
        return self._projectName

    @property
    def ProjectNameSignal(self) -> Signal:
        return self._projectNameSignal

    @property
    def ProjectPath(self) -> str:
        return self._projectPath if self._projectPath != "" else os.getcwd()

    @property
    def ProjectPathSignal(self) -> Signal:
        return self._projectPathSignal

    @property
    def FinalProjectName(self) -> str:
        base = os.getcwd() if self._projectPath == "" else self._projectPath
        return os.path.normpath(os.path.join(base, self._projectName))

    def SetOnConfirm(self, callback: ConfirmCallback) -> None:
        self._onConfirm = callback

    def Confirm(self) -> None:
        if self._onConfirm is not None:
            self._onConfirm(self)

    def Clear(self) -> None:
        self._projectName = ""
        self._projectNameSignal.Emit("")
        self._projectPath = ""
        self._projectPathSignal.Emit("")


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
        self._SetupUI()

    def _SetupUI(self) -> None:
        self.ui.projectNameInput.textChanged.connect(self._OnProjectNameChanged)
        self.viewModel.ProjectNameSignal.Attach(self._UpdateFinalProjectLabel)
        self.viewModel.ProjectNameSignal.Emit(self.viewModel.ProjectName)

        self.ui.browseButton.clicked.connect(self._OnBrowse)
        self.viewModel.ProjectPathSignal.Attach(self._UpdateProjectDirLabel)
        self.viewModel.ProjectPathSignal.Attach(self._UpdateFinalProjectLabel)
        self.viewModel.ProjectPathSignal.Emit(self.viewModel.ProjectPath)

        self.ui.buttonBox.accepted.connect(self._OnConfirm)
        self.ui.buttonBox.rejected.connect(self._OnCancel)

    def _OnProjectNameChanged(self, text: str) -> None:
        self.viewModel.ProjectNameSignal.Emit(text)

    def _UpdateFinalProjectLabel(self, text: str) -> None:
        self.ui.finalProjectName.setText(f"Project: {self.viewModel.FinalProjectName}")

    def _UpdateProjectDirLabel(self, text: str) -> None:
        self.ui.projectDirLabel.setText(text)

    def _OnBrowse(self) -> None:
        folder = QFileDialog.getExistingDirectory(
            self,
            "Select Project Directory",
            ".",
            QFileDialog.Option.ShowDirsOnly,
        )

        if folder:
            self.viewModel.ProjectPathSignal.Emit(folder)

    def _OnConfirm(self) -> None:
        self.viewModel.Confirm()
        self._UIClear()

    def _OnCancel(self) -> None:
        self.viewModel.Clear()
        self._UIClear()

    def _UIClear(self) -> None:
        self.ui.projectNameInput.setText("")
        self.ui.projectDirLabel.setText("")
