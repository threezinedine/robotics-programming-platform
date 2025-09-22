import clang.cindex
from .py_object import PyObject
from .py_field import PyField


class PyStruct(PyObject):
    """
    Represents a Python structure parsed from C/C++ code.
    """

    def __init__(self, cursor: clang.cindex.Cursor) -> None:
        super().__init__(cursor)
        self.name = cursor.spelling
        self.fields: list[PyField] = []

        for child in cursor.get_children():
            if child.kind == clang.cindex.CursorKind.FIELD_DECL:
                self.fields.append(PyField(child))

    def __repr__(self) -> str:
        return f"<PyStruct name='{self.name}'/>"
