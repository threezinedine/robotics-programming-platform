import clang.cindex
from .py_object import PyObject


class PyField(PyObject):
    """
    Represents a field within a Python structure parsed from C/C++ code.
    """

    def __init__(self, cursor: clang.cindex.Cursor) -> None:
        super().__init__(cursor)
        self.name = cursor.spelling
        self.type = cursor.type.spelling
        self.access = cursor.access_specifier.name.lower()

    def __repr__(self) -> str:
        return f"<PyField name='{self.name}' type='{self.type}'/>"
