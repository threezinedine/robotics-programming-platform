from warnings import filterwarnings

filterwarnings("ignore", category=DeprecationWarning)

from PyQt6.QtWidgets import QApplication, QMainWindow
from PyQt6 import QtCore
from converted_uis.main_window_ui import Ui_MainWindow


class MyMainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.ui = Ui_MainWindow()
        self.ui.setupUi(self)

    def keyPressEvent(self, a0):
        if a0.key() == QtCore.Qt.Key.Key_Escape:  # Escape key
            self.close()
        super().keyPressEvent(a0)


class MyApplication(QApplication):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.main_window = MyMainWindow()


def main():
    import sys

    app = MyApplication(sys.argv)
    app.main_window.showMaximized()
    sys.exit(app.exec())


if __name__ == "__main__":
    main()
