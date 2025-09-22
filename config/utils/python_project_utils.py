import os
import subprocess
from .path_utils import GetAbosolutePythonExecutable, GetAbsoluteVirtualEnvDir
from ..logger import logger
from ..constants import Constants
from .validation_utils import ValidateCommandExists


def InstallPackages(
    projectDir: str,
    packages: list[str],
) -> None:
    """
    Used for installing python libraries into a specified virtual environment.

    Parameters
    ----------
    projectDir : str
        The project directory where the virtual environment is located (relative to the `ABSOLUTE_BASE_DIR`).

    packages : list[str]
        The list of packages to install.
    """

    pythonExe = GetAbosolutePythonExecutable(projectDir)
    cwd = os.path.join(Constants.ABSOLUTE_BASE_DIR, projectDir)

    try:
        logger.info(f"Installing packages in '{projectDir}': {', '.join(packages)}")
        subprocess.run(
            [
                pythonExe,
                "-m",
                "pip",
                "install",
            ]
            + packages,
            check=True,
            shell=True,
            capture_output=True,
            cwd=cwd,
        )
        logger.info("Packages installed successfully.")

        logger.info("Update the requirements.txt file...")
        subprocess.run(
            [
                pythonExe,
                "-m",
                "pip",
                "freeze",
                ">",
                "requirements.txt",
            ],
            check=True,
            shell=True,
            capture_output=True,
            cwd=cwd,
        )
        logger.info("requirements.txt file updated successfully.")

    except subprocess.CalledProcessError as e:
        logger.error(f"Failed to install packages: {e.stderr.decode().strip()}")
        raise RuntimeError(
            f"Failed to install packages: {e.stderr.decode().strip()}"
        ) from e


def RunPythonProject(projectDir: str) -> None:
    """
    Runs the specified Python project.

    Parameters
    ----------
    projectDir : str
        The project directory where the main.py file is located (relative to the `ABSOLUTE_BASE_DIR`).
    """

    pythonExe = GetAbosolutePythonExecutable(projectDir)
    cwd = os.path.join(Constants.ABSOLUTE_BASE_DIR, projectDir)
    mainScript = os.path.join(cwd, "main.py")

    if not os.path.isfile(mainScript):
        raise FileNotFoundError(f"No main.py file found in '{projectDir}'.")

    args: list[str] = []

    if projectDir == "autogen":
        ValidateCommandExists("clang")

        clangPathResult = subprocess.run(
            [
                "where",
                "clang",
            ],
            check=True,
            shell=True,
            capture_output=True,
        )

        clangPath = clangPathResult.stdout.decode().strip()
        if not clangPath:
            raise FileNotFoundError("Clang executable not found in PATH.")

        clangPathDll = clangPath.replace("clang.exe", "libclang.dll")

        logger.debug(f"Using Clang library at: {clangPathDll}")

        coreHeaderFile = os.path.join(
            Constants.ABSOLUTE_BASE_DIR,
            "libraries",
            "core",
            "include",
            "core",
            "core.h",
        )

        args = [
            "--clang-path",
            clangPathDll,
            "--input",
            coreHeaderFile,
        ]

    try:
        logger.info(f"Running Python project in '{projectDir}'...")
        subprocess.run(
            [
                pythonExe,
                mainScript,
            ]
            + args,
            check=True,
            shell=True,
            cwd=cwd,
        )
        logger.info(f"Python project '{projectDir}' finished successfully.")

    except Exception as e:
        logger.error(f"Failed to run Python project: {e}")
        raise RuntimeError(f"Failed to run Python project: {e}") from e


def RunPythonProjectTest(projectDir: str) -> None:
    """
    Runs the specified Python project in test mode.

    Parameters
    ----------
    projectDir : str
        The project directory where the main.py file is located (relative to the `ABSOLUTE_BASE_DIR`).
    """

    pytestExe = os.path.join(
        GetAbsoluteVirtualEnvDir(projectDir),
        "Scripts",
        "pytest.exe",
    )
    cwd = os.path.join(Constants.ABSOLUTE_BASE_DIR, projectDir)

    try:
        logger.info(f"Running Python project in '{projectDir}' in test mode...")
        subprocess.run(
            [
                pytestExe,
            ],
            check=True,
            shell=True,
            cwd=cwd,
        )
        logger.info(f"Python project '{projectDir}' finished successfully.")

    except Exception as e:
        logger.error(f"Failed to run Python project: {e}")
        raise RuntimeError(f"Failed to run Python project: {e}") from e
