import pytest  # type: ignore
from .utils import GenerateFuncType, AssertGenerateResult


def test_bind_function(generateFunc: GenerateFuncType) -> None:
    result = generateFunc(
        """
/// @brief Add two integers
int RPP_E2E_BINDING Add(int a, int b);
""",
        "cpp_function_binding.j2",
        [],
    )

    expected = """
m.def("Add", &Add , "Add two integers" );
"""

    AssertGenerateResult(expected, result)
