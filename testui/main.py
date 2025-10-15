from warnings import filterwarnings

filterwarnings("ignore", category=DeprecationWarning)

import os
from PyQt6.QtWidgets import QApplication, QMainWindow
from PyQt6 import QtCore
from pyqttoast import Toast, ToastPreset, ToastPosition
from typing import Literal, Self, TypeAlias
from converted_uis.main_window_ui import Ui_MainWindow
from data import Application


APPLICATION_DATA_FILE = "app.json"  # will be ignored in .gitignore

ToastType: TypeAlias = Literal["info", "success", "warning", "error"]


class MyMainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.ui = Ui_MainWindow()
        self.ui.setupUi(self)

        self.Setup()

    def Setup(self) -> None:
        self.ui.openVSCodeAction.triggered.connect(self._OpenVSCode)

    def _OpenVSCode(self) -> None:
        assert MyApplication.instance is not None, "MyApplication instance is None"

        if MyApplication.applicationData is None:
            MyApplication.instance.ShowToast(
                "Application data is not loaded.",
                type="error",
            )
            return

        vscodePath = MyApplication.applicationData.vscodePath

        if vscodePath == "":
            MyApplication.instance.ShowToast(
                "VSCode path is not set. Please set it in app.json.",
                type="error",
            )
            return

        if not os.path.exists(vscodePath):
            MyApplication.instance.ShowToast(
                f"VSCode path '{vscodePath}' does not exist. Please set it in app.json.",
                type="error",
            )
            return

    def keyPressEvent(self, a0):
        if a0.key() == QtCore.Qt.Key.Key_Escape:  # Escape key
            self.close()
        super().keyPressEvent(a0)


class MyApplication(QApplication):
    applicationData: Application | None = None
    instance: Self | None = None

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.main_window = MyMainWindow()

        MyApplication.instance = self

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
    import sys

    app = MyApplication(sys.argv)
    app.main_window.showMaximized()
    sys.exit(app.exec())


if __name__ == "__main__":
    main()
