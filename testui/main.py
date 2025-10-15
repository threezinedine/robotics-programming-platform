from PyQt6.QtWidgets import QApplication, QMainWindow
from converted_uis.main_window_ui import Ui_MainWindow


class MyApplication(QApplication):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.main_window = QMainWindow()
        self.ui = Ui_MainWindow()
        self.ui.setupUi(self.main_window)


def main():
    import sys

    app = MyApplication(sys.argv)
    app.main_window.showMaximized()
    sys.exit(app.exec())


if __name__ == "__main__":
    main()
