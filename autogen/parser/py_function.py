import clang.cindex
from .py_object import PyObject
from .py_parameter import PyParameter


class PyFunction(PyObject):
    """
    Represents a Python function parsed from C/C++ code.
    """

    def __init__(self, cursor: clang.cindex.Cursor) -> None:
        super().__init__(cursor)
        self.return_type = cursor.result_type.spelling
        self.parameters: list[PyParameter] = []

        for child in cursor.get_children():
            print(child.kind, child.spelling)
            if child.kind == clang.cindex.CursorKind.PARM_DECL:
                self.parameters.append(PyParameter(child))

    def __repr__(self) -> str:
        return f"<PyFunction name='{self.name}' return_type='{self.return_type}'/>"
