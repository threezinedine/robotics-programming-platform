import os
import subprocess
import shutil

from .path_utils import GetAbsoluteBuildDir, CreateRecursiveDirIfNotExists
from .cache_file_utils import IsFileModified, UpdateFileCache
from config.utils.validation_utils import ValidateCommandExists

from .path_utils import GetAbsoluteBuildDir
from ..logger import logger
from ..constants import Constants
from .run_command import RunCommand


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


def BuildCPPPropertiesJson(folderDir: str) -> None:
    """
    Because the pybind11 need Python.h which is located based on the current Python environment,
    we need to generate the c_cpp_properties.json file for VSCode intellisense to work properly.
    This function will create the c_cpp_properties.json file in the given folder directory from the
    c_cpp_properties.json.in template file.

    Notes
    -----
    If the c_cpp_properties.json.in file does not exist, this function will do nothing.

    If the c_cpp_properties.json file already exists, nothing will be done.

    Parameters
    ----------
    folderDir : str
        The folder directory where the c_cpp_properties.json file will be created (relative to the `ABSOLUTE_BASE_DIR`).
    """
    folder = os.path.join(
        Constants.ABSOLUTE_BASE_DIR,
        folderDir,
    )

    vsfolder = os.path.join(
        folder,
        ".vscode",
    )

    if not os.path.exists(vsfolder):
        return

    outputPath = os.path.join(vsfolder, "c_cpp_properties.json")

    templatePath = os.path.join(vsfolder, "c_cpp_properties.json.in")
    if not os.path.exists(templatePath):
        return

    if not IsFileModified(templatePath) and os.path.exists(outputPath):
        return

    templateContent = ""

    with open(templatePath, "r", encoding="utf-8") as templateFile:
        templateContent = templateFile.read()

    pythonIncludeDir: None | str = None

    try:
        if Constants.IsLinuxPlatform():
            result = RunCommand(f"find /usr/ -name Python.h", capture=True)
            assert result is not None
            pythonExes = result.splitlines()
            assert len(pythonExes) > 0, "No python executable found."
            pythonIncludeDir = os.path.dirname(pythonExes[0])
        else:
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

        assert pythonIncludeDir is not None, "Failed to find python include directory."

        templateContent = templateContent.replace(
            "@@PYTHON_PATH@@", f"{pythonIncludeDir}"
        )

    except Exception as e:
        logger.error(f"Failed to find python executable: {e}")
        raise RuntimeError("Failed to find python executable.") from e

    logger.info("Generating c_cpp_properties.json...")

    with open(outputPath, "w", encoding="utf-8") as outputFile:
        outputFile.write(templateContent)

    UpdateFileCache(templatePath)


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

    if Constants.IsWindowsPlatform():
        makefile = "Visual Studio 17 2022"
    elif Constants.IsLinuxPlatform():
        makefile = "Unix Makefiles"
    else:
        raise RuntimeError("Unsupported platform for building C/C++ projects.")

    if os.path.exists(buildDir) and recreate:
        logger.info(f"Recreating build directory '{buildDir}'...")
        shutil.rmtree(buildDir)

    finalOptions: list[str] = []
    if buildOptions:
        for option in buildOptions:
            finalOptions.append(f"-D{option}")

    globalPythonExe = GetGlobalPythonExePath()
    globalPythonExe = globalPythonExe.replace("\\", "/")
    pythonHome = os.path.dirname(globalPythonExe)

    try:
        logger.info(f"Building project in '{projectDir}'...")
        cmakeGenerateCommand = (
            f'cmake -S {absoluteProjectDir} -B {buildDir} -G "{makefile}" '
            f" -DCMAKE_BUILD_TYPE={cmakeBuildType} -DRPP_PYTHON_HOME={pythonHome} {' '.join(finalOptions)}"
        )

        RunCommand(cmakeGenerateCommand, cwd=absoluteProjectDir)

        logger.info("Compiling the project...")
        buildCommand = f"cmake --build {buildDir} --config {cmakeBuildType}"
        RunCommand(buildCommand, cwd=absoluteProjectDir, capture=False)

    except Exception as e:
        logger.error(f"Failed to build project in '{projectDir}': {e}")
        raise RuntimeError(f"Failed to build project in '{projectDir}'.") from e


def RunCppProject(projectDir: str, projectType: str, memoryCheck: bool = False) -> None:
    """
    Runs the compiled C/C++ project (assumes the project has been built already).

    Parameters
    ----------
    projectDir : str
        The project directory where the CMakeLists.txt file is located (relative to the `ABSOLUTE_BASE_DIR`).

    projectType : str, optional
        The build type which is either 'dev' or 'prod'. Default is 'dev'.

    memoryCheck : bool, optional
        Whether to check for memory leaks using Valgrind (only on Linux). Default is False, on windows, the memory check
        is performed using code built-in memory tracking, see `platforms/include/platforms/memory.h`. and this flag is ignored.
    """

    buildDir = GetAbsoluteBuildDir(projectDir, projectType)
    cmakeBuildType = "Debug" if projectType == "dev" else "Release"

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
        logger.info(f"Building project in '{projectDir}'...")
        buildCommand = f"cmake --build {buildDir} --config {cmakeBuildType}"
        RunCommand(buildCommand, cwd=buildDir)

        if Constants.IsLinuxPlatform() and memoryCheck:
            ValidateCommandExists("valgrind")
            memoryLeakCheckCommand = f"valgrind {executable}"
            logger.info("Checking for memory leaks using Valgrind...")
            RunCommand(memoryLeakCheckCommand, cwd=buildDir)
        else:
            logger.info(f"Running project '{projectDir}'...")
            RunCommand(executable, cwd=buildDir)

    except Exception as e:
        logger.error(f"Failed to run project '{projectDir}': {e}")
        raise RuntimeError(f"Failed to run project '{projectDir}'.") from e


def RunCppProjectTest(projectDir: str, projectType: str) -> None:
    """
    Run the testing suite for a C/C++ project. The testing version will be run.

    Parameters
    ----------
    projectDir : str
        The project directory where the CMakeLists.txt file is located (relative to the `ABSOLUTE_BASE_DIR`).

    projectType : str, optional
        The build type which is either 'dev' or 'prod'. Default is 'dev'.
    """
    buildDir = GetAbsoluteBuildDir(projectDir, projectType)
    cmakeBuildType = "Debug" if projectType == "dev" else "Release"
    executableDir: str | None = None

    # create temp e2e folder

    # Prepare e2e directory
    e2eDir = os.path.join(Constants.ABSOLUTE_BASE_DIR, f"e2e-{projectDir}")
    CreateRecursiveDirIfNotExists(os.path.join(e2eDir, "Engine"))
    CreateRecursiveDirIfNotExists(os.path.join(e2eDir, "TestReports"))

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

    try:
        logger.info(f"Building project in '{projectDir}'...")
        buildCommand = f"cmake --build {buildDir} --config {cmakeBuildType}"
        RunCommand(buildCommand, cwd=buildDir)

        logger.info(f"Running tests for project '{projectDir}'...")
        RunCommand(f"{executable}", cwd=buildDir)
    except Exception as e:
        logger.error(f"Failed to run tests for project '{projectDir}': {e}")
        raise RuntimeError(f"Failed to run tests for project '{projectDir}'.") from e


def RunLibrariesTest(
    projectDir: str, projectType: str, filter: str | None = None
) -> None:
    """
    Runs components tests in the `libraries` project (assumes the project has been built already).

    Parameters
    ----------
    projectDir : str
        The project directory where the CMakeLists.txt file is located (relative to the `ABSOLUTE_BASE_DIR`).

    projectType : str, optional
        The build type which is either 'dev' or 'prod'. Default is 'dev'.

    filter : str, optional
        An optional filter to pass to the test executable to run specific tests only.
    """

    buildDir = GetAbsoluteBuildDir("libraries", projectType)
    cmakeBuildType = "Debug" if projectType == "dev" else "Release"

    if Constants.IsWindowsPlatform():
        executableDir = os.path.join(buildDir, projectDir, cmakeBuildType)
    else:
        executableDir = os.path.join(
            buildDir,
            projectDir,
        )

    try:
        logger.info(f"Building project in '{projectDir}'...")
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

        logger.info(f"Running project '{projectDir}'...")
        logger.debug(f"Executing command: {command}")
        subprocess.run(
            command,
            cwd=buildDir,
        )
    except Exception as e:
        logger.error(f"Failed to run project '{projectDir}': {e}")
        raise RuntimeError(f"Failed to run project '{projectDir}'.") from e
