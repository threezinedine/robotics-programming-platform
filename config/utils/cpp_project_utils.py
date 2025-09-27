import os
import subprocess
import shutil

from .path_utils import GetAbsoluteBuildDir
from ..logger import logger
from ..constants import Constants


def BuildProject(
    projectDir: str,
    projectType: str = "dev",
    recreate: bool = False,
    buildOptions: list[str] | None = None,
) -> None:
    """
    Used for building C/C++ projects using CMake.

    Parameters
    ----------
    projectDir : str
        The project directory where the CMakeLists.txt file is located (relative to the `ABSOLUTE_BASE_DIR`).

    projectType : str, optional
        The build type which is either 'dev' or 'prod'. Default is 'dev'.

    recreate : bool, optional
        Whether to recreate the build directory if it already exists. Default is False.

    buildOptions : list[str], optional
        Additional options to pass to the CMake command during the build process.
    """

    absoluteProjectDir = os.path.join(Constants.ABSOLUTE_BASE_DIR, projectDir)
    buildDir = GetAbsoluteBuildDir(projectDir, projectType)

    cmakeBuildType = "Debug" if projectType == "dev" else "Release"
    makefile = "Visual Studio 17 2022"

    if os.path.exists(buildDir) and recreate:
        logger.info(f"Recreating build directory '{buildDir}'...")
        shutil.rmtree(buildDir)

    finalOptions: list[str] = []
    if buildOptions:
        for option in buildOptions:
            finalOptions.append(f"-D{option}")

    try:
        logger.info(f"Building project in '{projectDir}'...")
        subprocess.run(
            [
                "cmake",
                "-S",
                absoluteProjectDir,
                "-B",
                buildDir,
                f"-G {makefile}",
                f"-DCMAKE_BUILD_TYPE={cmakeBuildType}",
            ]
            + finalOptions,
            check=True,
            shell=True,
            cwd=absoluteProjectDir,
        )

        logger.info("Compiling the project...")
        subprocess.run(
            [
                "cmake",
                "--build",
                buildDir,
                "--config",
                cmakeBuildType,
            ],
            check=True,
            shell=True,
            cwd=absoluteProjectDir,
        )

    except Exception as e:
        logger.error(f"Failed to build project in '{projectDir}': {e}")
        raise RuntimeError(f"Failed to build project in '{projectDir}'.") from e


def RunCppProject(projectDir: str, projectType: str) -> None:
    """
    Runs the compiled C/C++ project (assumes the project has been built already).

    Parameters
    ----------
    projectDir : str
        The project directory where the CMakeLists.txt file is located (relative to the `ABSOLUTE_BASE_DIR`).

    projectType : str, optional
        The build type which is either 'dev' or 'prod'. Default is 'dev'.
    """

    buildDir = GetAbsoluteBuildDir(projectDir, projectType)
    cmakeBuildType = "Debug" if projectType == "dev" else "Release"
    executableDir = os.path.join(buildDir, cmakeBuildType)

    # find executable file
    files = os.listdir(executableDir)
    executable = None
    for file in files:
        if file.endswith(".exe") and not file.endswith("tests.exe"):
            executable = os.path.join(executableDir, file)
            break

    if executable is None:
        raise FileNotFoundError(f"No executable found in '{executableDir}'.")

    try:
        logger.info(f"Building project in '{projectDir}'...")
        subprocess.run(
            [
                "cmake",
                "--build",
                buildDir,
                "--config",
                cmakeBuildType,
            ],
            check=True,
            shell=True,
            cwd=buildDir,
        )

        logger.info(f"Running project '{projectDir}'...")
        subprocess.run(
            [executable],
            check=True,
            shell=True,
            cwd=buildDir,
        )
    except Exception as e:
        logger.error(f"Failed to run project '{projectDir}': {e}")
        raise RuntimeError(f"Failed to run project '{projectDir}'.") from e


def RunLibrariesTest(projectDir: str, projectType: str) -> None:
    """
    Runs components tests in the `libraries` project (assumes the project has been built already).

    Parameters
    ----------
    projectDir : str
        The project directory where the CMakeLists.txt file is located (relative to the `ABSOLUTE_BASE_DIR`).

    projectType : str, optional
        The build type which is either 'dev' or 'prod'. Default is 'dev'.
    """

    buildDir = GetAbsoluteBuildDir("libraries", projectType)
    cmakeBuildType = "Debug" if projectType == "dev" else "Release"
    executableDir = os.path.join(buildDir, projectDir, cmakeBuildType)

    # find executable file
    files = os.listdir(executableDir)
    executable = None
    for file in files:
        if file.endswith("tests.exe"):
            executable = os.path.join(executableDir, file)
            break

    if executable is None:
        raise FileNotFoundError(f"No executable found in '{executableDir}'.")

    try:
        logger.info(f"Building project in '{projectDir}'...")
        subprocess.run(
            [
                "cmake",
                "--build",
                buildDir,
                "--config",
                cmakeBuildType,
            ],
            check=True,
            shell=True,
            cwd=buildDir,
        )

        logger.info(f"Running project '{projectDir}'...")
        subprocess.run(
            [executable],
            cwd=buildDir,
        )
    except Exception as e:
        logger.error(f"Failed to run project '{projectDir}': {e}")
        raise RuntimeError(f"Failed to run project '{projectDir}'.") from e
