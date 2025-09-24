import pytest  # type: ignore
from parser import Parse, Structure
from .utils import ParserContentWrapper
from .assertion import (
    EnumConstantsAssert,
    EnumAssert,
    FieldAssert,
    FunctionAssert,
    StructAssert,
    MethodAssert,
    ParameterAssert,
    ClassAssert,
)


def WrapperParse(inputContent: str, includeLibs: list[str] = []) -> Structure:
    result: Structure = {
        "enums": [],
        "structs": [],
        "functions": [],
        "classes": [],
    }

    Parse(
        "dummy.h",
        result,
        testContent=ParserContentWrapper(inputContent, includeLibs),
    )

    return result


def test_parse_enum():
    result = WrapperParse(
        """
/// @brief Color enum
enum RPP_PYTHON_BINDING RPP_JAVASCRIPT_BINDING Color {
    RED,
    GREEN, ///< Green color
    BLUE RPP_HIDE = 10,
};
    """
    )
    assert "enums" in result
    assert isinstance(result["enums"], list)
    assert len(result["enums"]) == 1

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
    ).Assert(result["enums"][0])


def test_parse_enum_no_constants():
    result = WrapperParse(
        """
enum RPP_PYTHON_BINDING EmptyEnum {
};
    """
    )
    EnumAssert(
        name="EmptyEnum",
        constants=[],
        annotations=["python"],
    ).Assert(result["enums"][0])


def test_parse_struct():
    result = WrapperParse(
        """
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
"""
    )

    assert "structs" in result
    assert isinstance(result["structs"], list)
    assert len(result["structs"]) == 1

    StructAssert(
        name="Point",
        fields=[
            FieldAssert(name="x", type="int", access="public", annotations=["hide"]),
            FieldAssert(name="y", type="int", access="public"),
            FieldAssert(name="protectedField", type="float", access="protected"),
            FieldAssert(name="z", type="int", access="private"),
        ],
        methods=[
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
        ],
        annotations=["python"],
    ).Assert(result["structs"][0])


def test_parse_function_simplest_function():
    result = WrapperParse(
        """
/**
    * @brief A simple hello world function.
    */
void RPP_PYTHON_BINDING HelloWorld();
"""
    )

    assert "functions" in result
    assert isinstance(result["functions"], list)
    assert len(result["functions"]) == 1

    FunctionAssert(
        name="HelloWorld",
        returnType="void",
        parameters=[],
        annotations=["python"],
        comment="A simple hello world function.",
    ).Assert(result["functions"][0])


def test_parse_function_function_with_parameters():
    result = WrapperParse(
        """
int RPP_PYTHON_BINDING Add(int a, int b);
"""
    )

    assert "functions" in result
    assert isinstance(result["functions"], list)
    assert len(result["functions"]) == 1

    FunctionAssert(
        name="Add",
        returnType="int",
        parameters=[
            ParameterAssert(name="a", type="int"),
            ParameterAssert(name="b", type="int"),
        ],
        annotations=["python"],
    ).Assert(result["functions"][0])


def test_parse_function_with_default_parameters():
    result = WrapperParse(
        """
/** @brief Multiplies two integers, with an optional second parameter.
    * @param a The first integer.
    * @param b The second integer, default is 2.
    * @return The product of a and b.
    */
int RPP_PYTHON_BINDING Multiply(int a, int b = 2);
"""
    )

    assert "functions" in result
    assert isinstance(result["functions"], list)
    assert len(result["functions"]) == 1

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
    ).Assert(result["functions"][0])


def test_parse_class():
    result = WrapperParse(
        """
class RPP_PYTHON_BINDING Calculator {
public:
    /// @brief Adds two integers.
    int Add(int a, int b);

    /// @brief Subtracts two integers.
    static int Subtract(int a, int b);

private:
    int lastResult;
};
"""
    )

    assert "classes" in result
    assert isinstance(result["classes"], list)
    assert len(result["classes"]) == 1

    ClassAssert(
        name="Calculator",
        methods=[
            MethodAssert(
                name="Add",
                returnType="int",
                access="public",
                comment="Adds two integers.",
                parameters=[
                    ParameterAssert(name="a", type="int"),
                    ParameterAssert(name="b", type="int"),
                ],
            ),
            MethodAssert(
                name="Subtract",
                returnType="int",
                access="public",
                comment="Subtracts two integers.",
                parameters=[
                    ParameterAssert(name="a", type="int"),
                    ParameterAssert(name="b", type="int"),
                ],
                isStatic=True,
            ),
        ],
        fields=[
            FieldAssert(name="lastResult", type="int", access="private"),
        ],
        annotations=["python"],
    ).Assert(result["classes"][0])


def test_parse_string_function():
    result = WrapperParse(
        """
#include <string>

std::string RPP_PYTHON_BINDING GetGreeting(const std::string& name);
""",
        ["string"],
    )

    assert "functions" in result
    assert isinstance(result["functions"], list)
    assert len(result["functions"]) == 1

    FunctionAssert(
        name="GetGreeting",
        returnType="std::string",
        parameters=[
            ParameterAssert(name="name", type="const std::string &"),
        ],
        annotations=["python"],
    ).Assert(result["functions"][0])
