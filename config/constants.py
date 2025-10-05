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

    @staticmethod
    def Setup() -> None:
        """
        Called at the start of the program to detect the platform and set up constants (os-dependent).
        """

        if os.name == "nt":  # Windows
            Constants.PLATFORM = "Windows"
            Constants.PYTHON_SCRIPT = "python"
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

    @staticmethod
    def IsWindowsPlatform() -> bool:
        return Constants.PLATFORM == "Windows"

    @staticmethod
    def IsLinuxPlatform() -> bool:
        return Constants.PLATFORM == "Linux"
