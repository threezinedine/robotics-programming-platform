import clang.cindex
from .py_function import PyFunction


class PyMethod(PyFunction):
    """
    Represents a method within a Python structure parsed from C/C++ code.
    """

    def __init__(self, cursor: clang.cindex.Cursor) -> None:
        super().__init__(cursor)
        self.access = cursor.access_specifier.name.lower()
        self.isStatic = cursor.is_static_method()
