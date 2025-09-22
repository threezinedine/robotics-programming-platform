import clang.cindex
from .py_object import PyObject
from .py_field import PyField
from .py_method import PyMethod


class PyStruct(PyObject):
    """
    Represents a Python structure parsed from C/C++ code.
    """

    def __init__(self, cursor: clang.cindex.Cursor) -> None:
        super().__init__(cursor)
        self.fields: list[PyField | PyMethod] = []

        for child in cursor.get_children():
            print(child.kind, child.spelling)
            if child.kind == clang.cindex.CursorKind.FIELD_DECL:
                self.fields.append(PyField(child))
            elif child.kind == clang.cindex.CursorKind.CXX_METHOD:
                self.fields.append(PyMethod(child))

    def __repr__(self) -> str:
        return f"<PyStruct name='{self.name}'/>"
