import clang.cindex
from .py_object import PyObject


class PyParameter(PyObject):
    """
    Represents a parameter within a method parsed from C/C++ code.
    """

    def __init__(self, cursor: clang.cindex.Cursor) -> None:
        super().__init__(cursor)
        self.type = cursor.type.spelling
        self.hasDefaultValue = False

        for child in cursor.get_children():
            if child.kind not in [
                clang.cindex.CursorKind.NAMESPACE_REF,
                clang.cindex.CursorKind.TYPE_REF,
            ]:
                self.hasDefaultValue = True

    def __repr__(self) -> str:
        return f"<PyParameter name='{self.name}' type='{self.type}'/>"
