import clang.cindex
from .py_object import PyObject
from .py_parameter import PyParameter


class PyFunction(PyObject):
    """
    Represents a Python function parsed from C/C++ code.
    """

    def __init__(self, cursor: clang.cindex.Cursor) -> None:
        super().__init__(cursor)
        self.returnType = cursor.result_type.spelling
        self.parameters: list[PyParameter] = []
        self.returnComment: str | None = None

        for child in cursor.get_children():
            if child.kind == clang.cindex.CursorKind.PARM_DECL:
                self.parameters.append(PyParameter(child))

        if cursor.raw_comment:
            self._ParseComments(cursor.raw_comment)

    def _ParseComments(self, comment: str) -> None:
        """
        Parses parameter comments from the function's comment.

        Parameters
        ----------
            comment (str)
                The raw comment string to parse.
        """
        import re

        paramPattern = re.compile(r"@param\s+(\w+)\s+(.*)")
        matches = paramPattern.findall(comment)

        returnPattern = re.compile(r"@return\s+(.*)")
        returnMatch = returnPattern.search(comment)

        if returnMatch:
            self.returnComment = returnMatch.group(1).strip()

        for paramName, paramComment in matches:
            for param in self.parameters:
                if param.name == paramName:
                    param.comment = paramComment.strip()

    def __repr__(self) -> str:
        return f"<PyFunction name='{self.name}' returnType='{self.returnType}'/>"
