"""
All utils for validating something (such as whether a command exists on the commandline, folder exists, ...)
"""

import os
import shutil
from typing import Any

from ..logger import logger
from ..constants import Constants
from .path_utils import GetAbosolutePythonExecutable
from .run_command import RunCommand


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
        RunCommand(f"{command} --version", capture=False)
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


def ValidateEnvDirExists(project: str, force: bool = False, **kwargs: Any) -> None:
    """
    Automatically creates the virtual environment for a given project if it does not exist.

    Parameters
    ----------
    project: str
        The project directory to check for the virtual environment (relative to the `ABSOLUTE_BASE_DIR`).

    force : bool, optional
        Whether to recreate the virtual environment if it already exists. Default is False.
    """
    from .path_utils import GetAbsoluteVirtualEnvDir

    venvDir = GetAbsoluteVirtualEnvDir(project)
    cwd = os.path.join(Constants.ABSOLUTE_BASE_DIR, project)
    pythonExe = GetAbosolutePythonExecutable(project)

    if os.path.exists(venvDir):
        if force:
            shutil.rmtree(venvDir)
        else:
            logger.debug(f"Virtual environment directory '{project}' exists.")
            return

    try:
        logger.info(f"Creating virtual environment in '{project}'...")
        createVenvCommand = f"{Constants.PYTHON_SCRIPT} -m venv venv"
        RunCommand(createVenvCommand, cwd=cwd)
        logger.info(f"Virtual environment created in '{project}'.")

        logger.info("Upgrading pip in the virtual environment...")
        upgradePipCommand = f"{pythonExe} -m pip install --upgrade pip"
        RunCommand(upgradePipCommand, cwd=cwd)
        logger.info("pip upgraded successfully.")

        logger.info("Installing dependencies in the virtual environment...")
        installDepsCommand = f"{pythonExe} -m pip install -r requirements.txt"
        RunCommand(installDepsCommand, cwd=cwd)
        logger.info("Dependencies installed successfully.")
    except Exception as e:
        logger.error(f"Failed to create virtual environment in '{project}': {e}")
        raise RuntimeError(
            f"Failed to create virtual environment in '{project}'."
        ) from e
