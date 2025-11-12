from config.dependency import InstallCppDependencies
from config.utils.validation_utils import ValidateCommandExists, ValidateEnvDirExists
from config.args import Args, CppProjectNames, PythonProjectNames
from config.constants import Constants
from config.utils.python_project_utils import (
    InstallPackages,
    RunPythonProject,
    RunPythonProjectTest,
)
from config.utils.cpp_project_utils import (
    BuildProject,
    RunCppProject,
    RunLibrariesTest,
    RunCppProjectTest,
)
from config.utils.configure_template_utils import GenerateTemplate


def main():
    args = Args()

    Constants.Setup()

    # ================== System Validation ==================
    ValidateCommandExists(Constants.PYTHON_SCRIPT)
    ValidateCommandExists("pip3")

    GenerateTemplate("gruntime/.vscode/c_cpp_properties.json.in")
    GenerateTemplate("gruntime/.vscode/tasks.json.in")
    GenerateTemplate("gruntime/.vscode/launch.json.in")

    ValidateEnvDirExists("templategen")
    RunPythonProject("templategen")

    if args.Project in PythonProjectNames:
        ValidateEnvDirExists(**args.ToDict())

        if args.IsPackageInstall:
            InstallPackages(**args.ToDict())

    if args.Project in CppProjectNames:
        ValidateCommandExists("cmake")
        ValidateCommandExists("git")
        InstallCppDependencies()

        if args.IsBuild:
            BuildProject(**args.ToDict())

    if args.IsTest:
        if args.Project in CppProjectNames:
            if args.Project == "libraries":
                RunLibrariesTest(**args.ToDict())
            else:
                RunCppProjectTest(**args.ToDict())
        else:
            RunPythonProjectTest(**args.ToDict())

    if args.IsRun:
        if args.Project in CppProjectNames:
            RunCppProject(**args.ToDict())
        else:
            RunPythonProject(**args.ToDict())


if __name__ == "__main__":
    main()
