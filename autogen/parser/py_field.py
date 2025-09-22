import clang.cindex


class PyField:
    """
    Represents a field within a Python structure parsed from C/C++ code.
    """

    def __init__(self, cursor: clang.cindex.Cursor) -> None:
        self.name = cursor.spelling
        self.type = cursor.type.spelling
        self.annotations: list[str] = []
        self.access = cursor.access_specifier.name.lower()

        for child in cursor.get_children():
            if child.kind == clang.cindex.CursorKind.ANNOTATE_ATTR:
                annotation = child.spelling
                self.annotations.append(annotation)

    def __repr__(self) -> str:
        return f"<PyField name='{self.name}' type='{self.type}'/>"
