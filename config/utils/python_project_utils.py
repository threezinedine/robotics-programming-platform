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
from .cpp_project_utils import BuildProject


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
            "include",
            "modules",
        ),
        os.path.join(
            Constants.ABSOLUTE_BASE_DIR,
            "libraries",
            "include",
            "core",
        ),
        os.path.join(
            Constants.ABSOLUTE_BASE_DIR,
            "libraries",
            "include",
            "applications",
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


def _GetUIFileList() -> list[str]:  # type: ignore
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
        "testui",
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

    if projectDir == "autogen":
        allHeaderFiles = _GetListOfHeaderFiles()
        allTemplateFiles = _GetListOfTemplateFiles()
        typeMapFiles = os.path.join(
            Constants.ABSOLUTE_BASE_DIR,
            "autogen",
            "type_map",
            "type_map_const.py",
        )

        librariesTempDir = os.path.join(
            Constants.ABSOLUTE_BASE_DIR,
            "libraries",
            "tmp",
        )

        writerOutput = os.path.join(
            librariesTempDir,
            "writer_output.cpp",
        )

        e2eOutput = os.path.join(
            librariesTempDir,
            "e2e_test_binding.cpp",
        )

        e2eJsonWriterBindingOutput = os.path.join(
            librariesTempDir,
            "e2e_json_writer_binding.cpp",
        )

        e2eAppendOutput = os.path.join(
            librariesTempDir,
            "e2e_test_append.cpp",
        )

        e2eJsonWriterAppendOutput = os.path.join(
            librariesTempDir,
            "e2e_json_writer_append.cpp",
        )

        e2ePythonModuleImportOutput = os.path.join(
            librariesTempDir,
            "e2e_python_module_import.cpp",
        )

        e2ePythonModuleCreateEnumOutput = os.path.join(
            librariesTempDir,
            "e2e_python_module_create_enum.cpp",
        )

        pyiE2EGRuntimeOutputDir = os.path.join(
            Constants.ABSOLUTE_BASE_DIR,
            "e2e",
            "packages",
        )

        CreateRecursiveDirIfNotExists(pyiE2EGRuntimeOutputDir)

        pyiE2EGRuntimeOutput = os.path.join(
            pyiE2EGRuntimeOutputDir,
            "__init__.pyi",
        )

        e2ePythonInitOutput = os.path.join(
            pyiE2EGRuntimeOutputDir,
            "__init__.py",
        )

        isAnyHeaderFileChanged = any(
            IsFileModified(headerFile)
            for headerFile in allHeaderFiles + allTemplateFiles + [typeMapFiles]
        )

        isWriterOutputFileExist = os.path.isfile(writerOutput)
        isE2EOutputFileExists = os.path.isfile(e2eOutput)
        isE2EJsonWriterBindingOutputFileExists = os.path.isfile(
            e2eJsonWriterBindingOutput
        )
        isPyiE2EGRuntimeFileExists = os.path.isfile(pyiE2EGRuntimeOutput)
        isE2EPythonInitFileExists = os.path.isfile(e2ePythonInitOutput)
        isE2EAppendOutputFileExists = os.path.isfile(e2eAppendOutput)
        isE2EJsonWriterAppendOutputFileExists = os.path.isfile(
            e2eJsonWriterAppendOutput
        )
        isE2EPythonModuleImportOutputFileExists = os.path.isfile(
            e2ePythonModuleImportOutput
        )
        # isPyiFileExists = os.path.isfile(pyiBindingOutput)
        # isCppBindingFileExists = os.path.isfile(cppBindingOutput)

        if (
            not isAnyHeaderFileChanged
            and isWriterOutputFileExist
            # and isPyiFileExists
            # and isCppBindingFileExists
            and isE2EOutputFileExists
            and isE2EJsonWriterBindingOutputFileExists
            and isPyiE2EGRuntimeFileExists
            and isE2EPythonInitFileExists
            and isE2EAppendOutputFileExists
            and isE2EJsonWriterAppendOutputFileExists
            and isE2EPythonModuleImportOutputFileExists
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
            clangPathResult = RunCommand(findClangCommand, capture=True)

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

        CreateRecursiveDirIfNotExists(librariesTempDir)

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

        e2eJsonWriterBindingOutputArgs = argCommon + [
            "--template",
            os.path.join(cwd, "templates", "e2e_json_writer_binding.j2"),
            "--output",
            e2eJsonWriterBindingOutput,
        ]

        e2eAppendOutputArgs = argCommon + [
            "--template",
            os.path.join(cwd, "templates", "e2e_module_register.j2"),
            "--output",
            e2eAppendOutput,
        ]

        e2eJsonWriterAppendOutputArgs = argCommon + [
            "--template",
            os.path.join(cwd, "templates", "e2e_json_writer_register.j2"),
            "--output",
            e2eJsonWriterAppendOutput,
        ]

        e2ePythonModuleImportOutputArgs = argCommon + [
            "--template",
            os.path.join(cwd, "templates", "e2e_module_import.j2"),
            "--output",
            e2ePythonModuleImportOutput,
        ]

        e2ePythonModuleEnumCreatedOutputArgs = argCommon + [
            "--template",
            os.path.join(cwd, "templates", "e2e_module_enum_create.j2"),
            "--output",
            e2ePythonModuleCreateEnumOutput,
        ]

        pyiE2EGRuntimeOutputArgs = argCommon + [
            "--template",
            os.path.join(cwd, "templates", "e2e_python_binding.j2"),
            "--output",
            pyiE2EGRuntimeOutput,
        ]

        e2eInitOutputArgs = argCommon + [
            "--template",
            os.path.join(cwd, "templates", "e2e_python_init_binding.j2"),
            "--output",
            e2ePythonInitOutput,
        ]

        logger.info("Header files have changed. Running autogen...")

        try:

            logger.info(f"Running Python project in '{projectDir}'...")

            e2eCommand = " ".join([pythonExe, mainScript] + e2eOutputArgs)
            RunCommand(e2eCommand, cwd=cwd)

            e2eJsonWriterCommand = " ".join(
                [pythonExe, mainScript] + e2eJsonWriterBindingOutputArgs
            )
            RunCommand(e2eJsonWriterCommand, cwd=cwd)

            writerOutputCommand = " ".join([pythonExe, mainScript] + writerOutputArgs)
            RunCommand(writerOutputCommand, cwd=cwd)

            pyiE2EGRuntimeCommand = f"{pythonExe} {mainScript} " + " ".join(
                pyiE2EGRuntimeOutputArgs
            )
            RunCommand(pyiE2EGRuntimeCommand, cwd=cwd)

            e2eInitCommand = f"{pythonExe} {mainScript} " + " ".join(e2eInitOutputArgs)
            RunCommand(e2eInitCommand, cwd=cwd)

            e2eAppendCommand = " ".join([pythonExe, mainScript] + e2eAppendOutputArgs)
            RunCommand(e2eAppendCommand, cwd=cwd)

            e2eJsonWriterAppendCommand = " ".join(
                [pythonExe, mainScript] + e2eJsonWriterAppendOutputArgs
            )
            RunCommand(e2eJsonWriterAppendCommand, cwd=cwd)

            e2ePythonModuleImportCommand = " ".join(
                [pythonExe, mainScript] + e2ePythonModuleImportOutputArgs
            )
            RunCommand(e2ePythonModuleImportCommand, cwd=cwd)

            e2ePythonModuleEnumCreatedCommand = " ".join(
                [pythonExe, mainScript] + e2ePythonModuleEnumCreatedOutputArgs
            )
            RunCommand(e2ePythonModuleEnumCreatedCommand, cwd=cwd)

            logger.info(f"Python project '{projectDir}' finished successfully.")

            for headerFile in allHeaderFiles + allTemplateFiles + [typeMapFiles]:
                UpdateFileCache(headerFile)

        except Exception as e:
            logger.error(f"Failed to run Python project: {e}")
            raise RuntimeError(f"Failed to run Python project: {e}") from e
    elif projectDir == "testui":
        try:
            BuildProject(
                "editor",
                projectType="dev",
                recreate=False,
            )

            logger.info(f"Running Python project in '{projectDir}'...")
            runCommand = f"{pythonExe} {mainScript}"
            RunCommand(runCommand, cwd=cwd)
            logger.info(f"Python project '{projectDir}' finished successfully.")

        except Exception as e:
            logger.error(f"Failed to run Python project: {e}")
            raise RuntimeError(f"Failed to run Python project: {e}") from e
    elif projectDir == "templategen":
        editorVSCodeDir = os.path.join(
            "editor",
            ".vscode",
        )
        librariesVSCodeDir = os.path.join(
            "libraries",
            ".vscode",
        )

        allTemplateFiles = [
            os.path.join(
                editorVSCodeDir,
                "c_cpp_properties.json.in",
            ),
            os.path.join(
                editorVSCodeDir,
                "launch.json.in",
            ),
            os.path.join(
                editorVSCodeDir,
                "tasks.json.in",
            ),
            os.path.join(
                librariesVSCodeDir,
                "c_cpp_properties.json.in",
            ),
            os.path.join(
                librariesVSCodeDir,
                "launch.json.in",
            ),
            os.path.join(
                librariesVSCodeDir,
                "tasks.json.in",
            ),
        ]

        allOutputFiles = [file[:-3] for file in allTemplateFiles]

        for templateFile, outputFile in zip(allTemplateFiles, allOutputFiles):
            finalTemplateFile = os.path.join(Constants.ABSOLUTE_BASE_DIR, templateFile)
            finalOutputFile = os.path.join(Constants.ABSOLUTE_BASE_DIR, outputFile)
            isOutputFileExist = os.path.exists(finalOutputFile)

            if not IsFileModified(templateFile) and not force and isOutputFileExist:
                logger.debug(f"Template file not modified, skipping: {templateFile}")
                continue

            try:
                logger.info(f"Running Python project in '{projectDir}'...")
                runCommand = f'{pythonExe} {mainScript} -i "{finalTemplateFile}" -o "{finalOutputFile}"'
                RunCommand(runCommand, cwd=cwd)
                logger.info(f"Python project '{projectDir}' finished successfully.")
                UpdateFileCache(templateFile)

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
        GetAbsoluteVirtualEnvDir("testui"),
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
            cwd=os.path.join(Constants.ABSOLUTE_BASE_DIR, "testui"),
        )
        logger.info("PyQt Designer tool launched successfully.")
    except Exception as e:
        logger.error(f"Failed to launch PyQt Designer tool: {e}")
        raise RuntimeError(f"Failed to launch PyQt Designer tool: {e}") from e
