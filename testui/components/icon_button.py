import os
from typing import Any, Callable
from PyQt6.QtWidgets import QPushButton
from PyQt6.QtCore import QSize
from PyQt6.QtGui import QIcon
from PyQt6.QtWidgets import QSizePolicy


class IconButton(QPushButton):
    """
    A button with an icon.

    Parameters
    ----------
    iconPath : str
        The path to the icon image file (with base "assets/images/").
    """

    def __init__(
        self,
        iconPath: str,
        function: Callable[[], None] | None = None,
        *args: Any,
        **kwargs: Any,
    ) -> None:
        super().__init__(*args, **kwargs)
        icon = QIcon()
        icon.addFile(
            os.path.join("assets", "images", iconPath),
            QSize(16, 16),
        )
        self.setText("")
        self.setIcon(icon)
        self.setSizePolicy(
            QSizePolicy.Policy.Minimum,
            QSizePolicy.Policy.Minimum,
        )

        if function is not None:
            self.clicked.connect(function)
