import clang.cindex
from .py_object import PyObject


class PyMethod(PyObject):
    """
    Represents a method within a Python structure parsed from C/C++ code.
    """

    def __init__(self, cursor: clang.cindex.Cursor) -> None:
        super().__init__(cursor)
        self.name = cursor.spelling
        self.return_type = cursor.result_type.spelling
        self.access = cursor.access_specifier.name.lower()

    def __repr__(self) -> str:
        return f"<PyMethod name='{self.name}' return_type='{self.return_type}'/>"
