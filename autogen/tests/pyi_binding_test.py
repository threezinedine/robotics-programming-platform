"""
Unitest for cpp_binding.j2 template (Just testing the separate modules here, the final cpp_binding.j2 is not tested).
"""

import pytest  # type: ignore
from .utils import GenerateFuncType, AssertGenerateResult


def test_bind_enum(generateFunc: GenerateFuncType) -> None:
    result = generateFunc(
        """
/// @brief Color enumeration
enum RPP_PYTHON_BINDING Color {
    RED,
    GREEN, ///< Green color
    BLUE RPP_HIDE = 10,
};
""",
        "pyi_enum_binding.j2",
        [],
    )

    expected = """
class Color(Enum):
    \"\"\"
        Color enumeration
    \"\"\"
    RED = 0
    GREEN = 1
    \"\"\"
        Green color
    \"\"\"
"""

    AssertGenerateResult(expected, result)


def test_non_bind_enum(generateFunc: GenerateFuncType) -> None:
    result = generateFunc(
        """
enum Color {
    RED,
    GREEN, ///< Green color
    BLUE RPP_HIDE = 10,
};
""",
        "pyi_enum_binding.j2",
        [],
    )

    expected = """
"""

    AssertGenerateResult(expected, result)


def test_binding_struct(generateFunc: GenerateFuncType) -> None:
    result = generateFunc(
        """
/// @brief Point structure
struct RPP_PYTHON_BINDING Point {
    int x; ///< X coordinate
    int y; ///< Y coordinate
    std::string label;
};
""",
        "pyi_struct_binding.j2",
        ["string"],
    )

    expected = """
class Point:
    \"\"\"
        Point structure
    \"\"\"
    x: int
    \"\"\"
        X coordinate
    \"\"\"
    y: int
    \"\"\"
        Y coordinate
    \"\"\"
    label: str
"""

    AssertGenerateResult(expected, result)


def test_non_binding_struct(generateFunc: GenerateFuncType) -> None:
    result = generateFunc(
        """
struct Point {
    int x; ///< X coordinate
    int y; ///< Y coordinate
    """,
        "pyi_struct_binding.j2",
        [],
    )

    expected = """
"""
    AssertGenerateResult(expected, result)


def test_not_bind_protected_or_private_fields(generateFunc: GenerateFuncType) -> None:
    result = generateFunc(
        """
struct RPP_PYTHON_BINDING Point {
    int x;
private:
    int y;
protected:
    int z;
};
""",
        "pyi_struct_binding.j2",
        [],
    )

    expected = """
class Point:
    x: int
"""

    AssertGenerateResult(expected, result)


def test_bind_class(generateFunc: GenerateFuncType) -> None:
    result = generateFunc(
        """
/// @brief Person class
class RPP_PYTHON_BINDING Person {
public:
    Person();

    /// @brief Get the name of the person
    std::string getName() const RPP_PYTHON_BINDING;
};
""",
        "pyi_class_binding.j2",
        ["string"],
    )

    expected = """
class Person:
    \"\"\"
        Person class
    \"\"\"
    def __init__(self) -> None: 
        ...

    def getName(self) -> str:
        \"\"\"
            Get the name of the person
        \"\"\"
        ...
"""

    AssertGenerateResult(expected, result)


def test_bind_class_with_fields(generateFunc: GenerateFuncType) -> None:
    result = generateFunc(
        """
class RPP_PYTHON_BINDING Person {
public:
    Person();

    std::string getName() const RPP_PYTHON_BINDING;
    void setName(const std::string& name) RPP_PYTHON_BINDING;

    int getAge() const;

    std::string name;
    int age;       ///< Age of the person

private:
    std::string secret;
protected:
    int id;
};
""",
        "pyi_class_binding.j2",
        ["string"],
    )

    expected = """
class Person:
    def __init__(self) -> None:
        ...

    def getName(self) -> str:
        ...

    def setName(self, name: str) -> None:
        ...

    name: str
    age: int
    \"\"\"
        Age of the person
    \"\"\"
"""

    AssertGenerateResult(expected, result)
