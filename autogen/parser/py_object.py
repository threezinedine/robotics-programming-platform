import clang.cindex


class PyObject:
    """
    All other parsed objects inherit from this base class.
    """

    def __init__(self, cursor: clang.cindex.Cursor) -> None:
        self.name = cursor.spelling
        self.annotations: list[str] = []

        for child in cursor.get_children():
            if child.kind == clang.cindex.CursorKind.ANNOTATE_ATTR:
                annotation = child.spelling
                self.annotations.append(annotation)

    def __repr__(self) -> str:
        return f"<PyObject/>"
