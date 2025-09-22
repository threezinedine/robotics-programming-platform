import clang.cindex
from .py_object import PyObject


class PyParameter(PyObject):
    """
    Represents a parameter within a method parsed from C/C++ code.
    """

    def __init__(self, cursor: clang.cindex.Cursor) -> None:
        super().__init__(cursor)
        self.type = cursor.type.spelling
        self.hasDefaultValue = len(list(cursor.get_children())) > 0

    def __repr__(self) -> str:
        return f"<PyParameter name='{self.name}' type='{self.type}'/>"
