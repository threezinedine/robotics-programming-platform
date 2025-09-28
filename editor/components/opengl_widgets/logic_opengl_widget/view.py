from PyQt6.QtWidgets import QWidget
from PyQt6.QtOpenGLWidgets import QOpenGLWidget
from PyQt6.QtCore import Qt
from Engine import Renderer, Line, Rectangle, Program
from utils.logger import logger  # type: ignore


class LogicOpenGLWidget(QOpenGLWidget):
    def __init__(
        self,
        parent: QWidget | None = None,
        flags: Qt.WindowType = Qt.WindowType.Widget,
    ):
        super().__init__(parent, flags=flags)
        self.renderer: int = -1
        self.line: Line | None = None
        self.rectangle: Rectangle | None = None
        self.program: Program | None = None

    def initializeGL(self) -> None:
        self.renderer = Renderer.CreateRenderer(
            self.width(), self.height(), "Logic OpenGL Widget"
        )
        logger.info(f"Created renderer with ID: {self.renderer}")

    #         self.program = Program(
    #             """
    # #version 330 core

    # layout(location = 0) in vec2 aPos;

    # void main()
    # {
    #     gl_Position = vec4(aPos, 0.0, 1.0);
    # }
    # """,
    #             """
    # #version 330 core
    # out vec4 FragColor;

    # void main()
    # {
    #     FragColor = vec4(1.0);
    # }
    #         """,
    #         )
    # self.rectangle = Rectangle()

    def resizeGL(self, w: int, h: int) -> None:
        # super().resizeGL(w, h)
        pass

    def paintGL(self) -> None:
        logger.debug("Painting LogicOpenGLWidget.")

        if self.renderer < 0:
            return

        Renderer.ActivateRenderer(self.renderer)
        # Renderer.PreDraw()

        # Renderer.PostDraw()

    def Close(self) -> None:
        logger.info("Close OpenGL Widget")

        if self.renderer >= 0:
            Renderer.DestroyRenderer(self.renderer)
