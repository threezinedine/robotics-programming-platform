"""
Unitest for cpp_binding.j2 template (Just testing the separate modules here, the final cpp_binding.j2 is not tested).
"""

import os
import pytest
from typing import Generator, Callable, TypeAlias
from generate import Generate
from .utils import ParserContentWrapper, AssertGenerateResult

templatePath = os.path.normpath(
    os.path.join(os.path.dirname(__file__), "..", "templates")
)

GenerateFuncType: TypeAlias = Callable[[str, str], str]


@pytest.fixture
def generateFunc() -> Generator[GenerateFuncType, None, None]:
    """
    The fixture which provides the Generate function (with setup and teardown).
    """

    def _generate(inputContent: str, templateFileName: str) -> str:
        inputFile = os.path.join(templatePath, "test_input.cpp")
        templateFile = os.path.join(templatePath, templateFileName)

        return Generate(inputFile, templateFile, inputContent)

    yield _generate


def test_bind_enum(generateFunc: GenerateFuncType) -> None:
    result = generateFunc(
        ParserContentWrapper(
            """
enum RPP_PYTHON_BINDING Color {
    RED,
    GREEN, ///< Green color
    BLUE RPP_HIDE = 10,
};
"""
        ),
        "pyi_enum_binding.j2",
    )

    expected = """
class Color(Enum):
    RED = 0
    GREEN = 1
"""

    AssertGenerateResult(expected, result)


def test_non_bind_enum(generateFunc: GenerateFuncType) -> None:
    result = generateFunc(
        ParserContentWrapper(
            """
enum Color {
    RED,
    GREEN, ///< Green color
    BLUE RPP_HIDE = 10,
};
"""
        ),
        "pyi_enum_binding.j2",
    )

    expected = """
"""

    AssertGenerateResult(expected, result)
