import clang.cindex
from clang.cindex import Cursor, TranslationUnit
from typing import Literal, TypeAlias

from parser.py_function import PyFunction

from .py_field import PyField
from .py_struct import PyStruct
from .py_enum import PyEnum, PyEnumConstant
from .py_method import PyMethod
from .py_parameter import PyParameter
from .py_class import PyClass

# Type alias for a list of Objects
CStruct: TypeAlias = (
    PyEnum
    | PyEnumConstant
    | PyStruct
    | PyField
    | PyMethod
    | PyParameter
    | PyFunction
    | PyClass
)

# Type alias for the overall structure
Structure: TypeAlias = dict[
    Literal[
        "enums",
        "structs",
        "functions",
        "classes",
    ],
    list[CStruct],
]


def Parse(
    inputFile: str,
    structure: Structure,
    testContent: str | None = None,
):
    """
    Main method to analyzing the C/C++ source code and extract structures.

    Parameters
    ----------
        inputFile (str)
            Path to the C/C++ source file to be analyzed.

        structure (Structure)
            The reference structure to be filled.

        testContent (str | None)
            Optional string content for testing purposes.
            This parameter should only be used in unit tests.

    Returns
    -------
        Structure: A dictionary containing lists of parsed structures.
    """

    finalInput = inputFile if testContent is None else "dummy.h"

    index = clang.cindex.Index.create()
    translationUnit: TranslationUnit = index.parse(
        finalInput,
        args=[
            "-x",
            "c++",
            "-std=c++17",
            "-I../libraries/core/include",
            "-I../libraries/modules/include",
            "-I../libraries/platforms/include",
        ],
        unsaved_files=(
            [
                (
                    (
                        "dummy.h",
                        testContent,
                    )
                ),
            ]
            if testContent is not None
            else []
        ),
    )
    cursor: Cursor = translationUnit.cursor

    for child in cursor.get_children():
        if (
            child.kind == clang.cindex.CursorKind.NAMESPACE and child.spelling == "rpp"
        ):  # only parse code in rpp namespace
            for c in child.get_children():
                if c.kind == clang.cindex.CursorKind.ENUM_DECL:
                    pyEnum = PyEnum(c)
                    structure["enums"].append(pyEnum)
                elif c.kind == clang.cindex.CursorKind.STRUCT_DECL:
                    pyStruct = PyStruct(c)
                    structure["structs"].append(pyStruct)
                elif c.kind == clang.cindex.CursorKind.FUNCTION_DECL:
                    pyClass = PyFunction(c)
                    structure["functions"].append(pyClass)
                elif c.kind == clang.cindex.CursorKind.CLASS_DECL:
                    pyClass = PyClass(c)
                    structure["classes"].append(pyClass)
