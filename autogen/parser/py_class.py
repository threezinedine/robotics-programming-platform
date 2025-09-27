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
        self.constructors: list[PyMethod] = []
        self.methods: list[PyMethod] = []
        self.fields: list[PyField] = []

        hasDefaultConstructor = False
        hasNoConstructor = True

        for child in cursor.get_children():
            if child.kind == clang.cindex.CursorKind.FIELD_DECL:
                self.fields.append(PyField(child))
            elif child.kind == clang.cindex.CursorKind.CXX_METHOD:
                self.methods.append(PyMethod(child))
            elif child.kind == clang.cindex.CursorKind.CONSTRUCTOR:
                hasNoConstructor = False
                childPyMethod = PyMethod(child)

                isCopyConstructor = False

                for param in childPyMethod.parameters:
                    if self.name in param.type:
                        isCopyConstructor = True
                        break

                if isCopyConstructor:
                    continue

                self.constructors.append(childPyMethod)
                if child.is_default_constructor():
                    hasDefaultConstructor = True

        self.hasDefaultConstructor = hasDefaultConstructor or hasNoConstructor

    def __repr__(self) -> str:
        return f"<PyClass name='{self.name}'/>"
