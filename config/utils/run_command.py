import subprocess
import typing
from ..logger import logger
from ..constants import Constants

def RunCommand(command: str, cwd: typing.Optional[str] = None, capture: bool = False) -> str | None:
    """
    The interface for running the command in the shell with the mimimum boilerplate.

    Parameters
    ----------
    command : str
        The command to run.
    cwd : typing.Optional[str], optional
        The working directory to run the command in. If None, uses the current working directory.
        The default is None, -> then the `ABSOLUTE_BASE_DIR` is used, the input folder path should
        be relative to the `ABSOLUTE_BASE_DIR`.

    capture : bool, optional
        Whether to capture the output of the command. The default is False.
    """

    logger.debug(f"Running command: {command}")

    appliedCwd = cwd if cwd is not None else Constants.ABSOLUTE_BASE_DIR

    try:
        if Constants.IsWindowsPlatform():
            result = subprocess.run(
                command.split(" "),
                check=True,
                shell=True,
                cwd=appliedCwd,
                capture_output=capture,
            )

            if capture:
                return result.stdout.decode("utf-8").strip() if result.stdout else None
            return None
        else:
            result = subprocess.run(
                command,
                check=True,
                shell=True,
                cwd=appliedCwd,
                capture_output=capture,
            )

            if capture:
                return result.stdout.decode("utf-8").strip() if result.stdout else None
            return None

    except subprocess.CalledProcessError as e:
        logger.error(f"Command '{command}' failed with exit code {e.returncode}.")
        raise e
    except Exception as e:
        logger.error(f"An error occurred while running command '{command}': {e}")
        raise e