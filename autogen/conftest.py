import pytest
import os
from tests.utils import ParserContentWrapper, GenerateFuncType
from typing import Generator
from generate import Generate

templatePath = os.path.normpath(os.path.join(os.path.dirname(__file__), "templates"))


@pytest.fixture
def generateFunc() -> Generator[GenerateFuncType, None, None]:
    """
    The fixture which provides the Generate function (with setup and teardown).
    """

    def _generate(
        inputContent: str,
        templateFileName: str,
        includeLibs: list[str] | None = None,
    ) -> str:
        includeLibs = includeLibs or []
        inputFile = os.path.join(templatePath, "test_input.cpp")
        templateFile = os.path.join(templatePath, templateFileName)

        return Generate(
            [inputFile],
            templateFile,
            ParserContentWrapper(inputContent, includeLibs),
        )

    yield _generate
