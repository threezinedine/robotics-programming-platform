import clang.cindex
from clang.cindex import Cursor, TranslationUnit
from typing import Literal, TypeAlias

from .py_field import PyField
from .py_struct import PyStruct
from .py_enum import PyEnum, PyEnumConstant

# Type alias for a list of Objects
CStruct: TypeAlias = PyEnum | PyEnumConstant | PyStruct | PyField

# Type alias for the overall structure
Structure: TypeAlias = dict[
    Literal[
        "enum",
        "struct",
        "function",
        "class",
    ],
    list[CStruct],
]


def Parse(inputFile: str, testContent: str | None = None) -> Structure:
    """
    Main method to analyzing the C/C++ source code and extract structures.

    Parameters
    ----------
        inputFile (str)
            Path to the C/C++ source file to be analyzed.

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
        args=["-x", "c++", "-std=c++17"],
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

    result: Structure = {
        "enum": [],
        "struct": [],
        "function": [],
        "class": [],
    }

    for child in cursor.get_children():
        if (
            child.kind == clang.cindex.CursorKind.NAMESPACE and child.spelling == "rpp"
        ):  # only parse code in rpp namespace
            for c in child.get_children():
                if c.kind == clang.cindex.CursorKind.ENUM_DECL:
                    pyEnum = PyEnum(c)
                    result["enum"].append(pyEnum)
                elif c.kind == clang.cindex.CursorKind.STRUCT_DECL:
                    pyStruct = PyStruct(c)
                    result["struct"].append(pyStruct)

    return result
