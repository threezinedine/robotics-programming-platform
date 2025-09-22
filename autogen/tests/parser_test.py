import pytest  # type: ignore
from parser import Parse
from parser.py_enum import PyEnum
from .utils import ParserContentWrapper
from .assertion import EnumConstantsAssert, EnumAssert


def test_parse_enum():
    result = Parse(
        "",
        testContent=ParserContentWrapper(
            """
namespace rpp {
    enum RPP_PYTHON_BINDING RPP_JAVASCRIPT_BINDING Color {
        RED,
        GREEN,
        BLUE = 10
    };
};
    """
        ),
    )
    assert "enum" in result
    assert isinstance(result["enum"], list)
    assert len(result["enum"]) == 1

    enum: PyEnum = result["enum"][0]  # type: ignore

    EnumAssert(
        name="Color",
        constants=[
            EnumConstantsAssert(name="RED", value=0),
            EnumConstantsAssert(name="GREEN", value=1),
            EnumConstantsAssert(name="BLUE", value=10),
        ],
        annotations=["python", "javascript"],
    ).Assert(enum)


def test_parse_enum_no_constants():
    result = Parse(
        "",
        testContent=ParserContentWrapper(
            """
namespace rpp {
    enum RPP_PYTHON_BINDING EmptyEnum {
    };
};
    """
        ),
    )
    enum: PyEnum = result["enum"][0]  # type: ignore

    EnumAssert(
        name="EmptyEnum",
        constants=[],
        annotations=["python"],
    ).Assert(enum)


def test_parse_struct():
    result = Parse(
        "",
        testContent=ParserContentWrapper(
            """
namespace rpp {
    struct Point {
        int x;
        int y;
    };
};
"""
        ),
    )

    assert "struct" in result
    assert isinstance(result["struct"], list)
    assert len(result["struct"]) == 1
