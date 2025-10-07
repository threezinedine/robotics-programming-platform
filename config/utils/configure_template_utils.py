import os
from .cache_file_utils import IsFileModified, UpdateFileCache
from ..constants import Constants
from ..logger import logger


def GenerateTemplate(templatePath: str) -> None:
    """
    With the given template path, if the template path has been modified, regenerate the template.

    Args:
        templatePath (str): The path to the template file. The output file will be generated in the same directory
            with the same name but without the .in extension. (e.g., template.txt.in -> template.txt)
            The input will be the relative path from the `ABSOLUTE_BASE_DIR`.
    """

    convertMap: dict[str, str] = {
        "PLATFORM": Constants.PLATFORM,
        "RPP_PLATFORM_DEFINE": "RPP_PLATFORM_" + Constants.PLATFORM.upper(),
        "PYTHON_INCLUDE_DIR": Constants.PYTHON_INCLUDE_DIR,
        "CPP_DEBUG_TYPE": Constants.CPP_DEBUG_TYPE,
        "PYTHON_SCRIPT": Constants.PYTHON_SCRIPT,
        "VSCODE_TASK_COMMAND": "sudo" if Constants.PLATFORM != "Windows" else "python",
        "VSCODE_FIRST_ARG": "python3" if Constants.PLATFORM != "Windows" else "",
    }

    fullPath = os.path.join(Constants.ABSOLUTE_BASE_DIR, templatePath)

    if not os.path.exists(fullPath) or not os.path.isfile(fullPath):
        raise FileNotFoundError(f"Template file not found: {fullPath}")

    if not fullPath.endswith(".in"):
        raise ValueError(f"Template file must have a .in extension: {fullPath}")

    outputFullPath = fullPath[: -len(".in")]  # Remove the .in extension

    if not IsFileModified(fullPath) and os.path.exists(outputFullPath):
        logger.debug(f"Template file not modified, skipping: {templatePath}")
        return

    with open(fullPath, "r", encoding="utf-8") as templateFile:
        templateContent = templateFile.read()

        for key, value in convertMap.items():
            templateContent = templateContent.replace(f"@@{key}@@", value)

        with open(outputFullPath, "w", encoding="utf-8") as outputFile:
            outputFile.write(templateContent)

        logger.info(f"Generated template: {outputFullPath}")
    UpdateFileCache(fullPath)
