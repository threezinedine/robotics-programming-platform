"""
Unitest for cpp_binding.j2 template (Just testing the separate modules here, the final cpp_binding.j2 is not tested).
"""

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
    )

    expected = """
"""

    AssertGenerateResult(expected, result)
