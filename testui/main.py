from warnings import filterwarnings

filterwarnings("ignore", category=DeprecationWarning)

import os
import sys
import subprocess
from PyQt6.QtWidgets import QApplication, QMainWindow, QTableWidgetItem
from PyQt6.QtGui import QKeyEvent
from PyQt6 import QtCore
from pyqttoast import Toast, ToastPreset, ToastPosition  # type: ignore
from typing import Any, Literal, Self, TypeAlias
from converted_uis.main_window_ui import Ui_MainWindow
from data import Application
from components.icon_button import IconButton
from functools import partial


APPLICATION_DATA_FILE = "app.json"  # will be ignored in .gitignore
CURRENT_DIR = os.path.dirname(os.path.abspath(__file__))
TEST_DIR = os.path.normpath(os.path.join(CURRENT_DIR, "..", "e2e"))

ToastType: TypeAlias = Literal["info", "success", "warning", "error"]


class MyMainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.ui = Ui_MainWindow()
        self.ui.setupUi(self)  # type: ignore

        self.Setup()

    def Setup(self) -> None:
        self.ui.openVSCodeAction.triggered.connect(self._OpenVSCode)

        self._LoadAllScripts()

    def _OpenVSCode(self) -> None:
        assert MyApplication.instance is not None, "MyApplication instance is None"

        if MyApplication.applicationData is None:
            MyApplication.instance.ShowToast(
                "Application data is not loaded.",
                type="error",
            )
            return

    def _LoadAllScripts(self) -> None:
        assert os.path.exists(TEST_DIR), f"Test directory {TEST_DIR} does not exist"
        scripts = os.listdir(TEST_DIR)
        scriptNames = [f[:-3] for f in scripts if f.endswith(".py")]

        self.ui.scenarioList.clearContents()

        for i in range(len(scriptNames)):
            self.ui.scenarioList.insertRow(i)
            self.ui.scenarioList.setItem(i, 0, QTableWidgetItem(scriptNames[i]))
            self.ui.scenarioList.setCellWidget(i, 1, IconButton("edit"))
            self.ui.scenarioList.setCellWidget(i, 2, IconButton("delete"))
            self.ui.scenarioList.setCellWidget(i, 3, IconButton("play"))
            self.ui.scenarioList.setCellWidget(
                i,
                4,
                IconButton(
                    "play",
                    partial(self._RunEditorTest, scriptNames[i]),
                ),
            )

        self.ui.scenarioList.resizeColumnsToContents()

    def _RunEditorTest(self, scenario: str) -> None:
        assert MyApplication.instance is not None, "MyApplication instance is None"
        assert (
            MyApplication.applicationData is not None
        ), "Application data is not loaded."

        editorTestExe = MyApplication.applicationData.editorTestExe

        if not os.path.exists(editorTestExe):
            MyApplication.instance.ShowToast(
                f"Editor test executable not found: {editorTestExe}",
                type="error",
            )
            return

        if not os.path.exists(os.path.join(TEST_DIR, f"{scenario}.py")):
            MyApplication.instance.ShowToast(
                f"Test script not found: {os.path.join(TEST_DIR, f'{scenario}.py')}",
                type="error",
            )
            return

        editorTestDir = os.path.dirname(editorTestExe)

        assert os.path.exists(
            editorTestDir
        ), f"Editor test directory not found: {editorTestDir}"

        assert os.path.exists(
            editorTestExe
        ), f"Editor test executable not found: {editorTestExe}"

        executable = os.path.relpath(editorTestExe, editorTestDir)

        try:
            subprocess.run(
                [executable, scenario],
                cwd=editorTestDir,
                check=True,
                shell=True,
            )
        except Exception as e:
            MyApplication.instance.ShowToast(
                f"Failed to run test: {e}",
                type="error",
            )
            return

    def keyPressEvent(self, a0: QKeyEvent) -> None:
        if a0.key() == QtCore.Qt.Key.Key_Escape:  # Escape key
            self.close()
        super().keyPressEvent(a0)


class MyApplication(QApplication):
    applicationData: Application | None = None
    instance: Self | None = None

    def __init__(self, *args: Any, **kwargs: Any):
        super().__init__(*args, **kwargs)
        self.main_window = MyMainWindow()

        MyApplication.instance = self  # type: ignore

        self.Setup()

    def Setup(self) -> None:
        if not os.path.exists(APPLICATION_DATA_FILE):
            MyApplication.applicationData = Application()
            MyApplication.applicationData.Save(APPLICATION_DATA_FILE)

        MyApplication.applicationData = Application.From(APPLICATION_DATA_FILE)

    def ShowToast(
        self,
        message: str,
        duration: int = 2000,
        type: ToastType = "info",
    ) -> None:
        preset = ToastPreset.INFORMATION
        if type == "success":
            preset = ToastPreset.SUCCESS
        elif type == "warning":
            preset = ToastPreset.WARNING
        elif type == "error":
            preset = ToastPreset.ERROR

        toast = Toast(self.main_window)
        toast.setText(message)
        toast.setDuration(duration)
        toast.applyPreset(preset)
        toast.setPosition(ToastPosition.TOP_RIGHT)
        toast.show()


def main():
    app = MyApplication(sys.argv)
    app.main_window.showMaximized()
    sys.exit(app.exec())


if __name__ == "__main__":
    main()
