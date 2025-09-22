import clang.cindex
from .py_object import PyObject
from .py_parameter import PyParameter


class PyMethod(PyObject):
    """
    Represents a method within a Python structure parsed from C/C++ code.
    """

    def __init__(self, cursor: clang.cindex.Cursor) -> None:
        super().__init__(cursor)
        self.return_type = cursor.result_type.spelling
        self.access = cursor.access_specifier.name.lower()
        self.parameters: list[PyParameter] = []

        for child in cursor.get_children():
            if child.kind == clang.cindex.CursorKind.PARM_DECL:
                self.parameters.append(PyParameter(cursor=child))

    def __repr__(self) -> str:
        return f"<PyMethod name='{self.name}' return_type='{self.return_type}'/>"
