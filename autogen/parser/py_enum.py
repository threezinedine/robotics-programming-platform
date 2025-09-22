import clang.cindex


class PyEnumConstant:
    """
    Represents a constant within a Python enumeration parsed from C/C++ code.
    """

    def __init__(self, cursor: clang.cindex.Cursor) -> None:
        self.name = cursor.spelling
        self.value = cursor.enum_value if cursor.enum_value else 0
        self.annotations: list[str] = []

        for child in cursor.get_children():
            print(child.kind, child.spelling)
            if child.kind == clang.cindex.CursorKind.ANNOTATE_ATTR:
                annotation = child.spelling
                self.annotations.append(annotation)

    def __repr__(self) -> str:
        return f"<PyEnumConstant name='{self.name}' value={self.value}/>"


class PyEnum:
    """
    Represents a Python enumeration parsed from C/C++ code.
    """

    def __init__(self, cursor: clang.cindex.Cursor) -> None:
        self.name = cursor.spelling
        self.annotations: list[str] = []
        self.constants: list[PyEnumConstant] = []

        for child in cursor.get_children():
            if child.kind == clang.cindex.CursorKind.ANNOTATE_ATTR:
                annotation = child.spelling
                self.annotations.append(annotation)
            elif child.kind == clang.cindex.CursorKind.ENUM_CONSTANT_DECL:
                self.constants.append(PyEnumConstant(cursor=child))

    def __repr__(self) -> str:
        return f"<PyEnum name='{self.name})'/>"
