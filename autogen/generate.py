import os
from jinja2 import Environment, FileSystemLoader
from parser import Parse, Structure


def Generate(
    inputFiles: list[str],
    templateFile: str,
    testContent: str | None = None,
) -> str:
    """
    Main method of the whole project which analyzes the C/C++ source code and create the code based
    on the provided template.

    Parameters
    ----------
        inputFiles (List[str])
            Paths to the C/C++ source files to be analyzed (absolute paths).

        templateFile (str)
            Path to the template file used for code generation (absolute path).

        testContent (str | None)
            Optional string content for testing purposes.

    Returns
    -------
        str: The generated code as a string.
    """

    templateDir = os.path.dirname(templateFile)
    templateName = os.path.basename(templateFile)

    env = Environment(loader=FileSystemLoader(templateDir))
    template = env.get_template(templateName)

    parser: Structure = {
        "enums": [],
        "structs": [],
        "functions": [],
        "classes": [],
    }

    for inputFile in inputFiles:
        Parse(inputFile, parser, testContent)

    return template.render(**parser)
