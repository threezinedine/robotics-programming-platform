import os
import subprocess
from .path_utils import (
    CreateRecursiveDirIfNotExists,
    GetAbosolutePythonExecutable,
    GetAbsolutePipExecutable,
    GetAbsoluteVirtualEnvDir,
)
from ..logger import logger
from ..constants import Constants
from .cache_file_utils import IsFileModified, UpdateFileCache
from .validation_utils import ValidateCommandExists
from .run_command import RunCommand


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

    pipExe = GetAbsolutePipExecutable(projectDir)
    cwd = os.path.join(Constants.ABSOLUTE_BASE_DIR, projectDir)

    try:
        logger.info(f"Installing packages in '{projectDir}': {', '.join(packages)}")

        installPackagesCommand = f"{pipExe} install " + " ".join(packages)

        RunCommand(installPackagesCommand, cwd=cwd)

        logger.info("Packages installed successfully.")

        logger.info("Update the requirements.txt file...")
        updateRequirementsCommand = f"{pipExe} freeze > requirements.txt"

        RunCommand(updateRequirementsCommand, cwd=cwd)

        logger.info("requirements.txt file updated successfully.")

    except subprocess.CalledProcessError as e:
        logger.error(f"Failed to install packages: {e.stderr.decode().strip()}")
        raise RuntimeError(
            f"Failed to install packages: {e.stderr.decode().strip()}"
        ) from e


def _GetListOfHeaderFiles() -> list[str]:
    """
    Retrieves all header files which ared used for generating the Python bindings.

    Returns
    -------
    list[str]
        A list of header file paths.
    """
    headerFiles: list[str] = []

    directories = [
        os.path.join(
            Constants.ABSOLUTE_BASE_DIR,
            "libraries",
            "modules",
            "include",
        ),
        os.path.join(
            Constants.ABSOLUTE_BASE_DIR,
            "libraries",
            "core",
            "include",
        ),
        os.path.join(
            Constants.ABSOLUTE_BASE_DIR,
            "libraries",
            "applications",
            "include",
        ),
    ]

    for directory in directories:
        for root, _, files in os.walk(directory):
            for file in files:
                if file.endswith(".h") or file.endswith(".hpp"):
                    headerFiles.append(os.path.join(root, file))

    return headerFiles


def _GetListOfTemplateFiles() -> list[str]:
    """
    Retrieves all template files which are used for generating the Python bindings.

    Returns
    -------
    list[str]
        A list of template file paths.
    """
    templateFiles: list[str] = []

    directories = [
        os.path.join(
            Constants.ABSOLUTE_BASE_DIR,
            "autogen",
            "templates",
        ),
    ]

    for directory in directories:
        for root, _, files in os.walk(directory):
            for file in files:
                if file.endswith(".j2"):
                    templateFiles.append(os.path.join(root, file))

    return templateFiles


def _GetUIFileList() -> list[str]:
    """
    Getting all the .ui file in the `assets/uis` directory (the output from Qt Designer).

    Returns
    -------
    list[str]
        A list of .ui file paths.
    """
    uiFiles: list[str] = []

    uiDirectory = os.path.join(
        Constants.ABSOLUTE_BASE_DIR,
        "editor",
        "assets",
        "uis",
    )

    for root, _, files in os.walk(uiDirectory):
        for file in files:
            if file.endswith(".ui"):
                uiFiles.append(os.path.join(root, file))

    return uiFiles


def RunPythonProject(
    projectDir: str,
    force: bool = False,
    reset: bool = False,
) -> None:
    """
    Runs the specified Python project.

    Parameters
    ----------
    projectDir : str
        The project directory where the main.py file is located (relative to the `ABSOLUTE_BASE_DIR`).

    force : bool
        If True, forces the execution of the project even if no relevant files have changed (default is False).

    reset : bool
        If True, resets the project state before running (default is False).
    """

    pythonExe = GetAbosolutePythonExecutable(projectDir)
    cwd = os.path.join(Constants.ABSOLUTE_BASE_DIR, projectDir)
    mainScript = os.path.join(cwd, "main.py")

    if not os.path.isfile(mainScript):
        raise FileNotFoundError(f"No main.py file found in '{projectDir}'.")

    # args: list[str] = []

    allHeaderFiles = _GetListOfHeaderFiles()
    allTemplateFiles = _GetListOfTemplateFiles()
    typeMapFiles = os.path.join(
        Constants.ABSOLUTE_BASE_DIR,
        "autogen",
        "type_map",
        "type_map_const.py",
    )

    if projectDir == "autogen":
        librariesTempDir = os.path.join(
            Constants.ABSOLUTE_BASE_DIR,
            "libraries",
            "tmp",
        )

        # cppBindingOutput = os.path.join(
        #     librariesTempDir,
        #     "binding.cpp",
        # )

        writerOutput = os.path.join(
            librariesTempDir,
            "writer_output.cpp",
        )

        e2eOutput = os.path.join(
            librariesTempDir,
            "e2e_test_binding.cpp",
        )

        # pyiBindingOutput = os.path.join(
        #     Constants.ABSOLUTE_BASE_DIR,
        #     "editor",
        #     "Engine",
        #     "core.pyi",
        # )

        pyiE2EGRuntimeOutput = os.path.join(
            Constants.ABSOLUTE_BASE_DIR,
            "e2e-gruntime",
            "TestSystem",
            "__init__.pyi",
        )

        isAnyHeaderFileChanged = any(
            IsFileModified(headerFile)
            for headerFile in allHeaderFiles + allTemplateFiles + [typeMapFiles]
        )

        isE2EOutputFileExists = os.path.isfile(e2eOutput)
        isPyiE2EGRuntimeFileExists = os.path.isfile(pyiE2EGRuntimeOutput)
        # isPyiFileExists = os.path.isfile(pyiBindingOutput)
        # isCppBindingFileExists = os.path.isfile(cppBindingOutput)

        if (
            not isAnyHeaderFileChanged
            # and isPyiFileExists
            # and isCppBindingFileExists
            and isE2EOutputFileExists
            and isPyiE2EGRuntimeFileExists
            and not reset
            and not force
        ):
            logger.info("No header files have changed. Skipping autogen.")
            return

        if Constants.IsWindowsPlatform():
            ValidateCommandExists("clang")

            findClangCommand = (
                "where clang" if Constants.IsWindowsPlatform() else "which clang"
            )
            clangPathResult = RunCommand(findClangCommand)

            assert clangPathResult is not None, "Failed to find clang executable."

            clangPath = clangPathResult.strip()
            if not clangPath:
                raise FileNotFoundError("Clang executable not found in PATH.")

            clangPathDll = clangPath.replace("clang.exe", "libclang.dll")
        else:
            clangPathDll = os.path.join(
                Constants.ABSOLUTE_BASE_DIR, "assets", "libclang.so"
            )

        logger.debug(f"Using Clang library at: {clangPathDll}")

        applicationHeaderFile = os.path.join(
            Constants.ABSOLUTE_BASE_DIR,
            "libraries",
            "applications",
            "include",
            "applications",
            "applications.h",
        )

        argCommon = [
            "--clang-path",
            clangPathDll,
            "--input",
            applicationHeaderFile,
        ]

        # args = argCommon + [
        #     "--template",
        #     os.path.join(cwd, "templates", "pyi_binding.j2"),
        #     "--output",
        #     pyiBindingOutput,
        # ]

        CreateRecursiveDirIfNotExists(librariesTempDir)

        # arg2 = argCommon + [
        #     "--template",
        #     os.path.join(cwd, "templates", "cpp_binding.j2"),
        #     "--output",
        #     cppBindingOutput,
        # ]

        writerOutputArgs = argCommon + [
            "--template",
            os.path.join(cwd, "templates", "writer_binding.j2"),
            "--output",
            writerOutput,
        ]

        e2eOutputArgs = argCommon + [
            "--template",
            os.path.join(cwd, "templates", "e2e_test_cpp_binding.j2"),
            "--output",
            e2eOutput,
        ]

        pyiE2EGRuntimeOutputArgs = argCommon + [
            "--template",
            os.path.join(cwd, "templates", "e2e_python_binding.j2"),
            "--output",
            pyiE2EGRuntimeOutput,
        ]

        logger.info("Header files have changed. Running autogen...")

        try:

            logger.info(f"Running Python project in '{projectDir}'...")
            # subprocess.run(
            #     [
            #         pythonExe,
            #         mainScript,
            #     ]
            #     + args,
            #     check=True,
            #     shell=True,
            #     cwd=cwd,
            # )

            # subprocess.run(
            #     [
            #         pythonExe,
            #         mainScript,
            #     ]
            #     + arg2,
            #     check=True,
            #     shell=True,
            #     cwd=cwd,
            # )

            e2eCommand = " ".join([pythonExe, mainScript] + e2eOutputArgs)
            RunCommand(e2eCommand, cwd=cwd)

            writerOutputCommand = " ".join([pythonExe, mainScript] + writerOutputArgs)
            RunCommand(writerOutputCommand, cwd=cwd)

            pyiE2EGRuntimeCommand = f"{pythonExe} {mainScript} " + " ".join(
                pyiE2EGRuntimeOutputArgs
            )
            RunCommand(pyiE2EGRuntimeCommand, cwd=cwd)

            logger.info(f"Python project '{projectDir}' finished successfully.")

            for headerFile in allHeaderFiles + allTemplateFiles + [typeMapFiles]:
                UpdateFileCache(headerFile)

        except Exception as e:
            logger.error(f"Failed to run Python project: {e}")
            raise RuntimeError(f"Failed to run Python project: {e}") from e
    elif projectDir == "editor":
        uiFiles = _GetUIFileList()
        uiFilesChanged: list[str] = list(filter(lambda f: IsFileModified(f), uiFiles))

        if reset or force:
            uiFilesChanged = uiFiles

        uicExe = os.path.join(
            GetAbsoluteVirtualEnvDir("editor"),
            "Scripts",
            "pyuic6.exe",
        )

        targetConvertedUiDir = os.path.join(
            Constants.ABSOLUTE_BASE_DIR,
            "editor",
            "converted_uis",
        )

        CreateRecursiveDirIfNotExists(targetConvertedUiDir)

        try:
            logger.info("Build the libraries project...")

            buildLibrariesCommand = f"{Constants.PYTHON_SCRIPT} config.py -p libraries build --options RPP_EDITOR=ON"
            RunCommand(buildLibrariesCommand)

            if len(uiFilesChanged) > 0:
                logger.info("Converting .ui files to .py files...")

                for uiFile in uiFilesChanged:
                    fileName = os.path.basename(uiFile)
                    targetUiFile = os.path.join(
                        targetConvertedUiDir,
                        fileName.replace(".ui", "_ui.py"),
                    )

                    try:
                        subprocess.run(
                            [
                                uicExe,
                                uiFile,
                                "-o",
                                targetUiFile,
                            ],
                            check=True,
                            shell=True,
                            cwd=cwd,
                        )
                    except Exception as e:
                        logger.warning(f"Failed to convert {uiFile}: {e}")

                logger.info("All .ui files are converted to .py files.")
            else:
                logger.info("No .ui files have changed. Skipping conversion.")

            logger.info(f"Running Python project in '{projectDir}'...")
            subprocess.run(
                [
                    pythonExe,
                    mainScript,
                ],
                check=True,
                shell=True,
                cwd=cwd,
            )
            logger.info(f"Python project '{projectDir}' finished successfully.")

            for uiFile in uiFilesChanged:
                UpdateFileCache(uiFile)

        except Exception as e:
            logger.error(f"Failed to run Python project: {e}")
            raise RuntimeError(f"Failed to run Python project: {e}") from e


def RunPythonProjectTest(projectDir: str, filter: str | None = None) -> None:
    """
    Runs the specified Python project in test mode.

    Parameters
    ----------
    projectDir : str
        The project directory where the main.py file is located (relative to the `ABSOLUTE_BASE_DIR`).

    filter : str | None
        Optional filter to pass to pytest to run specific tests only (default is None).
    """

    if Constants.IsWindowsPlatform():
        pytestExe = os.path.join(
            GetAbsoluteVirtualEnvDir(projectDir),
            "Scripts",
            "pytest.exe",
        )
    else:
        pytestExe = os.path.join(
            GetAbsoluteVirtualEnvDir(projectDir),
            "bin",
            "pytest",
        )

    cwd = os.path.join(Constants.ABSOLUTE_BASE_DIR, projectDir)

    try:
        logger.info(f"Running Python project in '{projectDir}' in test mode...")
        runTestCommand = f"{pytestExe} " + (f"-k {filter}" if filter else "")
        RunCommand(runTestCommand, cwd=cwd, capture=False)
        logger.info(f"Python project '{projectDir}' finished successfully.")

    except Exception as e:
        logger.error(f"Failed to run Python project: {e}")
        raise RuntimeError(f"Failed to run Python project: {e}") from e


def OpenPyQtDesigner() -> None:
    """
    Opens the PyQt Designer tool.
    """

    designgerExe = os.path.join(
        GetAbsoluteVirtualEnvDir("editor"),
        "Scripts",
        "pyqt6-tools.exe",
    )

    try:
        logger.info("Launching the PyQt Designer tool...")
        subprocess.run(
            [
                designgerExe,
                "designer",
            ],
            check=True,
            shell=True,
            cwd=os.path.join(Constants.ABSOLUTE_BASE_DIR, "editor"),
        )
        logger.info("PyQt Designer tool launched successfully.")
    except Exception as e:
        logger.error(f"Failed to launch PyQt Designer tool: {e}")
        raise RuntimeError(f"Failed to launch PyQt Designer tool: {e}") from e
