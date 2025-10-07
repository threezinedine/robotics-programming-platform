import os
import subprocess
from typing import Literal


class Constants:
    """
    The place where all constants are defined.
    """

    CONFIGURE_LOGGER_NAME = "CONFIGURE"

    # Project directories
    ABSOLUTE_BASE_DIR = os.getcwd()

    AUTOGEN_DIR = "autogen"
    ABSOLUTE_AUTOGEN_DIR = os.path.join(ABSOLUTE_BASE_DIR, AUTOGEN_DIR)

    E2E_DIR = "e2e"
    ABSOLUTE_E2E_DIR = os.path.join(ABSOLUTE_BASE_DIR, E2E_DIR)
    PYTHON_SCRIPT = "python"

    PLATFORM: Literal["Windows", "Linux", "macOS", "Unknown"] = "Windows"

    PYTHON_INCLUDE_DIR = ""

    CPP_DEBUG_TYPE = ""

    @staticmethod
    def Setup() -> None:
        """
        Called at the start of the program to detect the platform and set up constants (os-dependent).
        """

        pythonIncludeDir: str | None = None

        if os.name == "nt":  # Windows
            Constants.PLATFORM = "Windows"
            Constants.PYTHON_SCRIPT = "python"

            result = subprocess.run(
                ["where", "python"],
                capture_output=True,
                text=True,
            )

            text = subprocess.run(
                ["where", "python"],
                check=True,
                shell=True,
                cwd=folder,
                capture_output=True,
                text=True,
            )
            pythonExes = text.stdout.splitlines()

            assert len(pythonExes) > 0, "No python executable found."

            chosenPythonExe = pythonExes[0]
            pythonDir = os.path.dirname(chosenPythonExe)
            pythonIncludeDir = os.path.join(pythonDir, "include")
            pythonIncludeDir = pythonIncludeDir.replace("\\", "/")

            Constants.CPP_DEBUG_TYPE = "cppvsdbg"

        else:  # POSIX (Linux, macOS, etc.)
            if os.uname().sysname == "Darwin":
                Constants.PLATFORM = "macOS"
            elif os.uname().sysname == "Linux":
                Constants.PLATFORM = "Linux"
            else:
                raise RuntimeError("Unsupported platform.")

            try:
                result = subprocess.run(
                    ["which", "python"],
                    capture_output=True,
                    text=True,
                    check=True,
                )
                Constants.PYTHON_SCRIPT = result.stdout.strip()
            except subprocess.CalledProcessError:
                Constants.PYTHON_SCRIPT = "python3"

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

            Constants.CPP_DEBUG_TYPE = "cppdbg"

        assert (
            pythonIncludeDir is not None
        ), "Failed to determine Python include directory."
        Constants.PYTHON_INCLUDE_DIR = pythonIncludeDir

    @staticmethod
    def IsWindowsPlatform() -> bool:
        return Constants.PLATFORM == "Windows"

    @staticmethod
    def IsLinuxPlatform() -> bool:
        return Constants.PLATFORM == "Linux"
