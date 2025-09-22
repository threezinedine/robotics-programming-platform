import clang.cindex
from .py_field import PyField


class PyStruct:
    """
    Represents a Python structure parsed from C/C++ code.
    """

    def __init__(self, cursor: clang.cindex.Cursor) -> None:
        self.name = cursor.spelling
        self.fields: list[PyField] = []
        self.annotations: list[str] = []

        for child in cursor.get_children():
            print(child.kind, child.spelling)
            if child.kind == clang.cindex.CursorKind.FIELD_DECL:
                self.fields.append(PyField(child))
            elif child.kind == clang.cindex.CursorKind.ANNOTATE_ATTR:
                annotation = child.spelling
                self.annotations.append(annotation)

    def __repr__(self) -> str:
        return f"<PyStruct name='{self.name}'/>"
