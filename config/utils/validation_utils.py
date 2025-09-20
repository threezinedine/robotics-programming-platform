"""
All utils for validating something (such as whether a command exists on the commandline, folder exists, ...)
"""

import os
import subprocess
import shutil

from ..logger import logger
from ..constants import Constants
from .path_utils import GetAbosolutePythonExecutable


def ValidateCommandExists(command: str, required: bool = True) -> None:
    """
    Used for checking whether a command (like `git`, `clang`, `msbuild`, ...) exists on the commandline. If not,
    the user will need to install and configure it before proceeding.

    Parameters
    ----------
    command : str
        The command to check for existence.
    required : bool, optional
        Whether the command is required. If True and the command does not exist, an exception will be raised.
        If False, a warning will be logged instead. Default is True.
    """

    try:
        logger.debug(f"Checking if command '{command}' exists...")
        subprocess.run(
            [command, "--version"],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            check=True,
            cwd=Constants.ABSOLUTE_BASE_DIR,
        )
        logger.debug(f"Command '{command}' exists.")
    except Exception as e:
        if required:
            logger.error(f"Required command '{command}' is not available: {e}")
            raise RuntimeError(f"Required command '{command}' is not available.") from e
        else:
            logger.warning(f"Optional command '{command}' is not available: {e}")


def ValidateDirectoryExists(dirPath: str, autoCreate: bool = True) -> None:
    """
    Used for checking whether a directory exists.

    Parameters
    ----------
    dirPath : str
        The directory path to check for existence (relative to the `ABSOLUTE_BASE_DIR`).
    autoCreate : bool, optional
        Whether to automatically create the directory if it does not exist. Default is True.
    """

    logger.debug(f"Checking if directory '{dirPath}' exists...")

    if os.path.exists(dirPath) and os.path.isdir(dirPath):
        logger.debug(f"Directory '{dirPath}' exists.")
    else:
        if autoCreate:
            os.makedirs(dirPath, exist_ok=True)
            logger.info(f"Directory '{dirPath}' did not exist, but has been created.")
        else:
            logger.warning(f"Directory '{dirPath}' does not exist.")


def ValidateEnvDirExists(projectDir: str, recreate: bool = False) -> None:
    """
    Automatically creates the virtual environment for a given project if it does not exist.

    Parameters
    ----------
    projectDir : str
        The project directory to check for the virtual environment (relative to the `ABSOLUTE_BASE_DIR`).

    recreate : bool, optional
        Whether to recreate the virtual environment if it already exists. Default is False.
    """
    from .path_utils import GetAbsoluteVirtualEnvDir

    venvDir = GetAbsoluteVirtualEnvDir(projectDir)
    cwd = os.path.join(Constants.ABSOLUTE_BASE_DIR, projectDir)
    pythonExe = GetAbosolutePythonExecutable(projectDir)

    if os.path.exists(venvDir):
        if recreate:
            shutil.rmtree(venvDir)
        else:
            logger.debug(f"Virtual environment directory '{projectDir}' exists.")
            return

    try:
        logger.info(f"Creating virtual environment in '{projectDir}'...")
        subprocess.run(
            [
                "python",
                "-m",
                "venv",
                "venv",
            ],
            check=True,
            shell=True,
            capture_output=True,
            cwd=cwd,
        )
        logger.info(f"Virtual environment created in '{projectDir}'.")

        logger.info("Upgrading pip in the virtual environment...")
        subprocess.run(
            [
                pythonExe,
                "-m",
                "pip",
                "install",
                "--upgrade",
                "pip",
            ],
            check=True,
            shell=True,
            capture_output=True,
            cwd=cwd,
        )
        logger.info("pip upgraded successfully.")

        logger.info("Installing dependencies in the virtual environment...")
        subprocess.run(
            [
                pythonExe,
                "-m",
                "pip",
                "install",
                "-r",
                "requirements.txt",
            ],
            check=True,
            shell=True,
            capture_output=True,
            cwd=cwd,
        )
        logger.info("Dependencies installed successfully.")
    except Exception as e:
        logger.error(f"Failed to create virtual environment in '{projectDir}': {e}")
        raise RuntimeError(
            f"Failed to create virtual environment in '{projectDir}'."
        ) from e
