import clang.cindex
from .py_object import PyObject
from .py_field import PyField
from .py_method import PyMethod


class PyClass(PyObject):
    """
    Represents a Python class parsed from C/C++ code.
    """

    def __init__(self, cursor: clang.cindex.Cursor) -> None:
        super().__init__(cursor)
        self.methods: list[PyMethod] = []
        self.fields: list[PyField] = []

        for child in cursor.get_children():
            if child.kind == clang.cindex.CursorKind.FIELD_DECL:
                self.fields.append(PyField(child))
            elif child.kind == clang.cindex.CursorKind.CXX_METHOD:
                self.methods.append(PyMethod(child))

    def __repr__(self) -> str:
        return f"<PyClass name='{self.name}'/>"
