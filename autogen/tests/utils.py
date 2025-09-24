from typing import Callable, TypeAlias


def ParserContentWrapper(text: str, includeLibs: list[str] = []) -> str:
    """
    Add some usedful macros like `RPP_PYTHON_BINDING`, `RPP_JAVASCRIPT_BINDING`, `RPP_BINDING` for testing purposes.

    Parameters
    ----------
        text (str)
            The text to be wrapped.

        includeLibs (list[str])
            The list of libraries to include. Default is an empty list.

    Returns
    -------
        str
            The wrapped text.
    """
    return f"""
#define RPP_PYTHON_BINDING __attribute__((annotate("python")))
#define RPP_JAVASCRIPT_BINDING __attribute__((annotate("javascript")))
#define RPP_BINDING __attribute__((annotate("python", "javascript")))

#define RPP_HIDE __attribute__((annotate("hide")))

{" ".join([f'#include <{lib}>' for lib in includeLibs]) if includeLibs else ''}

namespace rpp {{
    {text}
}};
"""


def AssertGenerateResult(expected: str, actual: str) -> None:
    """
    Used for checking the equality of the generated code, the code may have different indentation or extra spaces.

    Parameters
    ----------
        expected (str)
            The expected code.

        actual (str)
            The actual code.


    """

    def compressed(s: str) -> str:
        return s.replace(" ", "").replace("\n", "")

    compressedExpected = compressed(expected)
    compressedActual = compressed(actual)

    assert (
        compressedExpected == compressedActual
    ), f"Expected:\n{expected}\n\nActual:\n{actual}"


GenerateFuncType: TypeAlias = Callable[[str, str, list[str] | None], str]
