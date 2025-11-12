from config.dependency import InstallCppDependencies
from config.utils.validation_utils import ValidateCommandExists, ValidateEnvDirExists
from config.args import Args, CppProjectNames, PythonProjectNames
from config.constants import Constants
from config.utils.python_project_utils import (
    InstallPackages,
    RunPythonProject,
    RunPythonProjectTest,
    OpenPyQtDesigner,
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
        ValidateEnvDirExists(
            projectDir=args.Project,
            recreate=args.IsForce,
        )

        if args.IsPackageInstall:
            InstallPackages(
                projectDir=args.Project,
                packages=args.InstalledPackages,
            )

    if args.Project in CppProjectNames:
        ValidateCommandExists("cmake")
        ValidateCommandExists("git")
        InstallCppDependencies()

        if args.IsBuild:
            BuildProject(
                projectDir=args.Project,
                args=args,
            )

    if args.IsTest:
        if args.Project in CppProjectNames:
            if args.Project == "libraries":
                RunLibrariesTest(
                    projectDir="core",
                    projectType=args.Type,
                    filter=args.TestFilter,
                )
            else:
                RunCppProjectTest(
                    projectDir=args.Project,
                    projectType=args.Type,
                    scenario=args.Scenario,
                    filter=args.TestFilter,
                )
        else:
            RunPythonProjectTest(
                projectDir=args.Project,
                filter=args.TestFilter,
            )

    if args.IsRun:
        if args.Project in CppProjectNames:
            RunCppProject(
                projectDir=args.Project,
                projectType=args.Type,
                memoryCheck=args.CheckMemoryLeaks,
            )
        else:
            RunPythonProject(
                projectDir=args.Project,
                force=args.IsForce,
                reset=args.IsReset,
            )

    if args.IsDesigner:
        if Constants.IsLinuxPlatform():
            raise Exception("The PyQt Designer tool is not supported on Linux.")
        OpenPyQtDesigner()


if __name__ == "__main__":
    main()
