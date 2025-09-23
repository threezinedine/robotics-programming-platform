import pytest  # type: ignore
from parser import Parse
from .utils import ParserContentWrapper
from .assertion import (
    EnumConstantsAssert,
    EnumAssert,
    FieldAssert,
    FunctionAssert,
    StructAssert,
    MethodAssert,
    ParameterAssert,
)


def test_parse_enum():
    result = Parse(
        "",
        testContent=ParserContentWrapper(
            """
namespace rpp {
    /// @brief Color enum
    enum RPP_PYTHON_BINDING RPP_JAVASCRIPT_BINDING Color {
        RED,
        GREEN, ///< Green color
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
            EnumConstantsAssert(
                name="GREEN",
                value=1,
                comment="Green color",
            ),
            EnumConstantsAssert(
                name="BLUE",
                value=10,
                annotations=["hide"],
            ),
        ],
        comment="Color enum",
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
        int GetX() const;
        void SetX(int newX);

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
            MethodAssert(name="GetX", returnType="int", access="protected"),
            MethodAssert(
                name="SetX",
                returnType="void",
                access="protected",
                parameters=[
                    ParameterAssert(name="newX", type="int"),
                ],
            ),
            FieldAssert(name="z", type="int", access="private"),
        ],
        annotations=["python"],
    ).Assert(result["struct"][0])


def test_parse_function_simplest_function():
    result = Parse(
        "",
        testContent=ParserContentWrapper(
            """
namespace rpp {
    /**
     * @brief A simple hello world function.
     */
    void RPP_PYTHON_BINDING HelloWorld();
};
"""
        ),
    )

    assert "function" in result
    assert isinstance(result["function"], list)
    assert len(result["function"]) == 1

    FunctionAssert(
        name="HelloWorld",
        returnType="void",
        parameters=[],
        annotations=["python"],
        comment="A simple hello world function.",
    ).Assert(result["function"][0])


def test_parse_function_function_with_parameters():
    result = Parse(
        "",
        testContent=ParserContentWrapper(
            """
namespace rpp {
    int RPP_PYTHON_BINDING Add(int a, int b);
};
"""
        ),
    )

    assert "function" in result
    assert isinstance(result["function"], list)
    assert len(result["function"]) == 1

    FunctionAssert(
        name="Add",
        returnType="int",
        parameters=[
            ParameterAssert(name="a", type="int"),
            ParameterAssert(name="b", type="int"),
        ],
        annotations=["python"],
    ).Assert(result["function"][0])


def test_parse_function_with_default_parameters():
    result = Parse(
        "",
        testContent=ParserContentWrapper(
            """
namespace rpp {
    /** @brief Multiplies two integers, with an optional second parameter.
     * @param a The first integer.
     * @param b The second integer, default is 2.
     * @return The product of a and b.
     */
    int RPP_PYTHON_BINDING Multiply(int a, int b = 2);
};
"""
        ),
    )

    assert "function" in result
    assert isinstance(result["function"], list)
    assert len(result["function"]) == 1

    FunctionAssert(
        name="Multiply",
        returnType="int",
        parameters=[
            ParameterAssert(
                name="a",
                type="int",
                comment="The first integer.",
            ),
            ParameterAssert(
                name="b",
                type="int",
                hasDefaultValue=True,
                comment="The second integer, default is 2.",
            ),
        ],
        annotations=["python"],
        comment="Multiplies two integers, with an optional second parameter.",
        returnComment="The product of a and b.",
    ).Assert(result["function"][0])
