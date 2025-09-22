import pytest  # type: ignore
from parser import Parse
from .utils import ParserContentWrapper
from .assertion import (
    EnumConstantsAssert,
    EnumAssert,
    FieldAssert,
    StructAssert,
    MethodAssert,
)


def test_parse_enum():
    result = Parse(
        "",
        testContent=ParserContentWrapper(
            """
namespace rpp {
    enum RPP_PYTHON_BINDING RPP_JAVASCRIPT_BINDING Color {
        RED,
        GREEN,
        BLUE RPP_HIDE = 10,
    };
};
    """
        ),
    )
    assert "enum" in result
    assert isinstance(result["enum"], list)
    assert len(result["enum"]) == 1

    EnumAssert(
        name="Color",
        constants=[
            EnumConstantsAssert(name="RED", value=0),
            EnumConstantsAssert(name="GREEN", value=1),
            EnumConstantsAssert(name="BLUE", value=10, annotations=["hide"]),
        ],
        annotations=["python", "javascript"],
    ).Assert(result["enum"][0])


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
    EnumAssert(
        name="EmptyEnum",
        constants=[],
        annotations=["python"],
    ).Assert(result["enum"][0])


def test_parse_struct():
    result = Parse(
        "",
        testContent=ParserContentWrapper(
            """
namespace rpp {
    struct RPP_PYTHON_BINDING Point {
        int x RPP_HIDE;
        int y;

    protected:
        float protectedField;

        void PrintHello();

    private:
        int z;
    };
};
"""
        ),
    )

    assert "struct" in result
    assert isinstance(result["struct"], list)
    assert len(result["struct"]) == 1

    StructAssert(
        name="Point",
        fields=[
            FieldAssert(name="x", type="int", access="public", annotations=["hide"]),
            FieldAssert(name="y", type="int", access="public"),
            FieldAssert(name="protectedField", type="float", access="protected"),
            MethodAssert(name="PrintHello", returnType="void", access="protected"),
            FieldAssert(name="z", type="int", access="private"),
        ],
        annotations=["python"],
    ).Assert(result["struct"][0])
