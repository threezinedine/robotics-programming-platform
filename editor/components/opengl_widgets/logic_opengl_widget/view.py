from PyQt6.QtWidgets import QWidget
from PyQt6.QtOpenGLWidgets import QOpenGLWidget
from PyQt6.QtCore import Qt
from Engine import Renderer, Line, Rectangle, Program, Rect
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
        self.rectangle: int = -1
        self.program: int = -1

    def initializeGL(self) -> None:
        self.renderer = Renderer.CreateRenderer(
            self.width(), self.height(), "Logic OpenGL Widget"
        )
        logger.info(f"Created renderer with ID: {self.renderer}")

        self.program = Program.Create(
            """
#version 330 core

layout(location = 0) in vec2 aPos;

void main()
{
    gl_Position = vec4(aPos, 0.0, 1.0);
}
""",
            """
#version 330 core
out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0);
}
        """,
        )

        self.rectangle = Rectangle.Create()

    def resizeGL(self, w: int, h: int) -> None:
        # super().resizeGL(w, h)
        pass

    def paintGL(self) -> None:
        logger.debug("Painting LogicOpenGLWidget.")

        if self.renderer < 0:
            return

        Renderer.ActivateRenderer(self.renderer)
        # Renderer.PreDraw()

        if self.program >= 0:
            Program.Use(self.program)

        if self.rectangle >= 0:
            rect = Rect()
            rect.centerX = 0.6
            rect.centerY = 0.0
            rect.width = 0.1
            rect.height = 0.1

            Rectangle.Draw(self.rectangle, rect)

        # Renderer.PostDraw()

    def Close(self) -> None:
        logger.info("Close OpenGL Widget")

        if self.renderer >= 0:
            Renderer.ActivateRenderer(self.renderer)

            if self.program >= 0:
                Program.Destroy(self.program)

            if self.rectangle >= 0:
                Rectangle.Destroy(self.rectangle)

            Renderer.DestroyRenderer(self.renderer)
