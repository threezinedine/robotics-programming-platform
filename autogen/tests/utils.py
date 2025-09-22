def ParserContentWrapper(text: str) -> str:
    """
    Add some usedful macros like `RPP_PYTHON_BINDING`, `RPP_JAVASCRIPT_BINDING`, `RPP_BINDING` for testing purposes.

    Parameters
    ----------
        text (str)
            The text to be wrapped.
    Returns
    -------
        str
            The wrapped text.
    """
    return f"""
#define RPP_PYTHON_BINDING __attribute__((annotate("python")))
#define RPP_JAVASCRIPT_BINDING __attribute__((annotate("javascript")))
#define RPP_BINDING __attribute__((annotate("python", "javascript")))

    {text}
"""
