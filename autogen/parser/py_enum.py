import clang.cindex
from .py_object import PyObject


class PyEnumConstant(PyObject):
    """
    Represents a constant within a Python enumeration parsed from C/C++ code.
    """

    def __init__(self, cursor: clang.cindex.Cursor) -> None:
        super().__init__(cursor)
        self.value = cursor.enum_value if cursor.enum_value else 0

    def __repr__(self) -> str:
        return f"<PyEnumConstant name='{self.name}' value={self.value}/>"


class PyEnum(PyObject):
    """
    Represents a Python enumeration parsed from C/C++ code.
    """

    def __init__(self, cursor: clang.cindex.Cursor) -> None:
        super().__init__(cursor)
        self.constants: list[PyEnumConstant] = []

        for child in cursor.get_children():
            if child.kind == clang.cindex.CursorKind.ENUM_CONSTANT_DECL:
                self.constants.append(PyEnumConstant(cursor=child))

    def __repr__(self) -> str:
        return f"<PyEnum name='{self.name})'/>"
