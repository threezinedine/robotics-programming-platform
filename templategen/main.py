import os
import argparse
import subprocess
from typing import Any
from jinja2 import Template


def main():
    parser = argparse.ArgumentParser(description="Project Management Utility")

    parser.add_argument(
        "--input-file",
        "-i",
        type=str,
        required=True,
    )

    parser.add_argument(
        "--output-file",
        "-o",
        type=str,
    )

    args = parser.parse_args()

    inputFile = args.input_file

    outputFile: str | None = None
    if inputFile.endswith(".in"):
        outputFile = args.output_file if args.output_file else inputFile[:-3]

    assert (
        outputFile is not None
    ), "Output file must be specified or input file must be .in files"

    assert os.path.exists(inputFile), f"Input file does not exist: {inputFile}"

    template: Template | None = None

    with open(inputFile, "r", encoding="utf-8") as file:
        template = Template(file.read())

    assert template is not None, "Template could not be created"

    data: dict[str, Any] = {}
    data["RPP_PROJECT_DIR"] = os.path.dirname(os.getcwd()).replace("\\", "/")

    if os.name == "nt":
        data["PLATFORM"] = "Windows"
        data["RPP_PLATFORM_DEFINE"] = "RPP_PLATFORM_WINDOWS"
        data["PYTHON_SCRIPT"] = "python"
        data["CPP_DEBUG_TYPE"] = "cppvsdbg"

        result = subprocess.run(
            ["where", "python"],
            capture_output=True,
            text=True,
        )

        text = subprocess.run(
            ["where", "python"],
            check=True,
            shell=True,
            cwd=os.path.dirname(os.getcwd()),
            capture_output=True,
            text=True,
        )
        pythonExes = text.stdout.splitlines()

        assert len(pythonExes) > 0, "No python executable found."

        chosenPythonExe = pythonExes[0]
        pythonDir = os.path.dirname(chosenPythonExe)
        pythonIncludeDir = os.path.join(pythonDir, "include")
        pythonIncludeDir = pythonIncludeDir.replace("\\", "/")

        data["PYTHON_INCLUDE_DIR"] = pythonIncludeDir
    else:
        data["PLATFORM"] = "Linux"
        data["RPP_PLATFORM_DEFINE"] = "RPP_PLATFORM_LINUX"
        data["PYTHON_SCRIPT"] = "python3"
        data["CPP_DEBUG_TYPE"] = "cppdbg"

        try:
            result = subprocess.run(
                ["find", "/usr/", "-name", "Python.h"],
                capture_output=True,
                text=True,
                check=True,
            )
            assert result is not None
            pythonIncludeDir = os.path.dirname(result.stdout.splitlines()[0])
        except Exception as e:
            raise RuntimeError(
                "Failed to find Python.h. Please ensure Python development headers are installed."
            ) from e

        assert (
            pythonIncludeDir is not None
        ), "Failed to determine Python include directory."
        data["PYTHON_INCLUDE_DIR"] = pythonIncludeDir

    renderedContent = template.render(**data)
    print(data, renderedContent)

    with open(outputFile, "w", encoding="utf-8") as file:
        print(f"Generating: {outputFile}")
        file.write(renderedContent)


if __name__ == "__main__":
    main()
