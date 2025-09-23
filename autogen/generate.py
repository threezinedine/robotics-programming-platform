import os
from jinja2 import Environment, FileSystemLoader
from parser import Parse


def Generate(inputFile: str, templateFile: str, testContent: str | None = None) -> str:
    """
    Main method of the whole project which analyzes the C/C++ source code and create the code based
    on the provided template.

    Parameters
    ----------
        inputFile (str)
            Path to the C/C++ source file to be analyzed (absolute path).

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

    parser = Parse(inputFile, testContent)

    return template.render(**parser)
