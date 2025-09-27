from PyQt6.QtWidgets import QWidget
from PyQt6.QtOpenGLWidgets import QOpenGLWidget


class LogicOpenGLWidget(QOpenGLWidget):
    def __init__(self, parent: QWidget | None = None):
        super().__init__(parent)
