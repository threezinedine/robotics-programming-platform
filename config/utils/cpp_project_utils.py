import os
import subprocess
import shutil

from .validation_utils import ValidateEnvDirExists

from .path_utils import GetAbsoluteBuildDir
from ..logger import logger
from ..constants import Constants
from .run_command import RunCommand
from typing import Any


def GetGlobalPythonExePath() -> str:
    """
    Get the global Python executable path using the `where` command.
    Returns
    -------
    str
        The global Python executable path.
    """
    try:
        if Constants.IsWindowsPlatform():
            text = subprocess.run(
                ["where", "python"],
                check=True,
                shell=True,
                cwd=Constants.ABSOLUTE_BASE_DIR,
                capture_output=True,
                text=True,
            )
            pythonExes = text.stdout.splitlines()
        else:
            result = RunCommand(f"which {Constants.PYTHON_SCRIPT}", capture=True)
            assert result is not None
            pythonExes = result.splitlines()

        assert len(pythonExes) > 0, "No python executable found."

        chosenPythonExe = pythonExes[0]
        return chosenPythonExe

    except Exception as e:
        logger.error(f"Failed to find python executable: {e}")
        raise RuntimeError("Failed to find python executable.") from e


def BuildProject(
    project: str,
    type: str = "dev",
    force: bool = False,
    options: list[str] | None = None,
    **kwargs: Any,
) -> None:
    """
    Used for building C/C++ projects using CMake.

    Parameters
    ----------
    project: str
        The project directory where the CMakeLists.txt file is located (relative to the `ABSOLUTE_BASE_DIR`).

    type : str, optional
        The build type which is either 'dev' or 'prod'. Default is 'dev'.

    force : bool, optional
        Whether to recreate the build directory if it already exists. Default is False.

    options : list[str], optional
        Additional CMake options to pass during the configuration step.
    """

    absoluteProjectDir = os.path.join(Constants.ABSOLUTE_BASE_DIR, project)
    buildDir = GetAbsoluteBuildDir(project, type)

    cmakeBuildType = "Debug" if type == "dev" else "Release"

    if Constants.IsWindowsPlatform():
        makefile = "Visual Studio 17 2022"
    elif Constants.IsLinuxPlatform():
        makefile = "Unix Makefiles"
    else:
        raise RuntimeError("Unsupported platform for building C/C++ projects.")

    if os.path.exists(buildDir) and force:
        logger.info(f"Recreating build directory '{buildDir}'...")
        shutil.rmtree(buildDir)

    finalOptions: list[str] = []

    if options:
        for option in options:
            finalOptions.append(f"-D{option}")

    globalPythonExe = GetGlobalPythonExePath()
    globalPythonExe = globalPythonExe.replace("\\", "/")
    pythonHome = os.path.dirname(globalPythonExe)

    if Constants.IsWindowsPlatform():
        RunCommand(
            f"{Constants.PYTHON_SCRIPT} config.py -p autogen run",
            cwd=Constants.ABSOLUTE_BASE_DIR,
        )
    else:
        RunCommand(
            f"{Constants.PYTHON_SCRIPT} config.py -p autogen run",
            cwd=Constants.ABSOLUTE_BASE_DIR,
        )

    try:
        project = Constants.ABSOLUTE_BASE_DIR.replace("\\", "/")

        logger.info(f"Building project in '{project}'...")
        cmakeGenerateCommand = (
            f'cmake -S {absoluteProjectDir} -B {buildDir} -G "{makefile}" '
            f" -DCMAKE_BUILD_TYPE={cmakeBuildType} -DRPP_PYTHON_HOME={pythonHome}"
            f' -DRPP_PROJECT_DIR="{project}"'
            f" {' '.join(finalOptions)}"
        )

        RunCommand(cmakeGenerateCommand, cwd=absoluteProjectDir)

        logger.info("Compiling the project...")
        buildCommand = f"cmake --build {buildDir} --config {cmakeBuildType}"
        RunCommand(buildCommand, cwd=absoluteProjectDir, capture=False)

    except Exception as e:
        logger.error(f"Failed to build project in '{project}': {e}")
        raise RuntimeError(f"Failed to build project in '{project}'.") from e


def RunCppProject(project: str, type: str, **kwargs: Any) -> None:
    """
    Runs the compiled C/C++ project (assumes the project has been built already).

    Parameters
    ----------
    project : str
        The project directory where the CMakeLists.txt file is located (relative to the `ABSOLUTE_BASE_DIR`).

    type : str, optional
        The build type which is either 'dev' or 'prod'. Default is 'dev'.
    """

    buildDir = GetAbsoluteBuildDir(project, type)
    cmakeBuildType = "Debug" if type == "dev" else "Release"

    if Constants.IsWindowsPlatform():
        RunCommand(
            f"{Constants.PYTHON_SCRIPT} config.py -p autogen run",
            cwd=Constants.ABSOLUTE_BASE_DIR,
        )
    else:
        RunCommand(
            f"{Constants.PYTHON_SCRIPT} config.py -p autogen run",
            cwd=Constants.ABSOLUTE_BASE_DIR,
        )

    if Constants.IsWindowsPlatform():
        executableDir = os.path.join(buildDir, cmakeBuildType)
    else:
        executableDir = buildDir

    # find executable file
    files = os.listdir(executableDir)
    executable = None
    if Constants.IsWindowsPlatform():
        for file in files:
            if file.endswith(".exe") and not file.endswith("test.exe"):
                executable = os.path.join(executableDir, file)
                break
    else:
        for file in files:
            fileCompletePath = os.path.join(executableDir, file)
            if (
                os.access(fileCompletePath, os.X_OK)
                and os.path.isfile(fileCompletePath)
                and not "test" in file
            ):
                executable = fileCompletePath
                break

    if executable is None:
        raise FileNotFoundError(f"No executable found in '{executableDir}'.")

    try:
        logger.info(f"Building project in '{project}'...")
        buildCommand = f"cmake --build {buildDir} --config {cmakeBuildType}"
        RunCommand(buildCommand, cwd=buildDir)

        RunCommand(executable, cwd=buildDir)

    except Exception as e:
        logger.error(f"Failed to run project '{project}': {e}")
        raise RuntimeError(f"Failed to run project '{project}'.") from e


def RunCppProjectTest(
    project: str,
    type: str = "dev",
    scenario: str | None = None,
    filter: str | None = None,
    **kwargs: Any,
) -> None:
    """
    Run the testing suite for a C/C++ project. The testing version will be run.

    Parameters
    ----------
    project: str
        The project directory where the CMakeLists.txt file is located (relative to the `ABSOLUTE_BASE_DIR`).

    type : str, optional
        The build type which is either 'dev' or 'prod'. Default is 'dev'.

    scenario : str, optional
        The scenario to use when running the tests. Only used for e2e testing (like `e2e-gruntime` or `e2e-editor`).

    filter : str, optional
        An optional filter to pass to the test executable to run specific tests only.
    """
    buildDir = GetAbsoluteBuildDir(project, type)
    cmakeBuildType = "Debug" if type == "dev" else "Release"
    executableDir: str | None = None

    if Constants.IsWindowsPlatform():
        executableDir = os.path.join(buildDir, cmakeBuildType)
    else:
        executableDir = buildDir

    # find executable file
    files = os.listdir(executableDir)
    executable = None

    if Constants.IsWindowsPlatform():
        for file in files:
            if file.endswith("test.exe"):
                executable = os.path.join(executableDir, file)
                break
    else:
        for file in files:
            fileCompletePath = os.path.join(executableDir, file)
            if (
                os.access(fileCompletePath, os.X_OK)
                and os.path.isfile(fileCompletePath)
                and "test" in file
            ):
                executable = fileCompletePath
                break

    if executable is None:
        raise FileNotFoundError(f"No executable found in '{executableDir}'.")

    assert executable is not None, f"No test executable found in '{executableDir}'."

    ValidateEnvDirExists(project="testui", force=False)

    try:
        logger.info(f"Building project in '{project}'...")
        buildCommand = f"cmake --build {buildDir} --config {cmakeBuildType}"
        RunCommand(buildCommand, cwd=buildDir)

        TEST_UI_DIR = os.path.join(Constants.ABSOLUTE_BASE_DIR, "testui")
        if os.name == "nt":
            TEST_UI_PYTHON_EXE = os.path.join(
                TEST_UI_DIR, "venv", "Scripts", "python.exe"
            )
        else:
            TEST_UI_PYTHON_EXE = os.path.join(TEST_UI_DIR, "venv", "bin", "python3")

        runCommand = f"{TEST_UI_PYTHON_EXE} main.py --project {project}"
        if scenario:
            runCommand += f" --scenario {scenario}"

        if filter:
            runCommand += f" --wildcard {filter}"

        print("Command to run tests:", runCommand)
        RunCommand(runCommand, cwd=TEST_UI_DIR)
    except Exception as e:
        logger.error(f"Failed to run tests for project '{project}': {e}")
        raise RuntimeError(f"Failed to run tests for project '{project}'.") from e


def RunLibrariesTest(
    type: str,
    filter: str | None = None,
    **kwargs: Any,
) -> None:
    """
    Runs components tests in the `libraries` project (assumes the project has been built already).

    Parameters
    ----------
    type : str, optional
        The build type which is either 'dev' or 'prod'. Default is 'dev'.

    filter : str, optional
        An optional filter to pass to the test executable to run specific tests only.
    """

    buildDir = GetAbsoluteBuildDir("libraries", type)
    cmakeBuildType = "Debug" if type == "dev" else "Release"

    if Constants.IsWindowsPlatform():
        executableDir = os.path.join(buildDir, cmakeBuildType)
    else:
        executableDir = os.path.join(
            buildDir,
        )

    try:
        logger.info(f"Building project in 'libraries'...")
        buildCommand = f"cmake --build {buildDir} --config {cmakeBuildType}"
        RunCommand(buildCommand, cwd=buildDir)

        # find executable file
        files = os.listdir(executableDir)
        executable = None
        for file in files:
            if Constants.IsWindowsPlatform():
                if file.endswith("tests.exe"):
                    executable = os.path.join(executableDir, file)
                    break
            else:
                fileCompletePath = os.path.join(executableDir, file)
                if (
                    os.access(fileCompletePath, os.X_OK)
                    and os.path.isfile(fileCompletePath)
                    and "tests" in file
                ):
                    executable = fileCompletePath
                    break

        if executable is None:
            raise FileNotFoundError(f"No executable found in '{executableDir}'.")

        command = f"{executable} --gtest_filter={filter}" if filter else executable

        logger.info(f"Running project 'libraries'...")
        logger.debug(f"Executing command: {command}")
        RunCommand(command, cwd=executableDir)
    except Exception as e:
        logger.error(f"Failed to run project 'libraries': {e}")
        raise RuntimeError(f"Failed to run project 'libraries'.") from e
