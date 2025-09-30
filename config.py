from config.dependency import InstallCppDependencies
from config.utils.validation_utils import ValidateCommandExists, ValidateEnvDirExists
from config.args import Args, CppProjectNames, PythonProjectNames
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
)


def main():
    args = Args()

    # ================== System Validation ==================
    ValidateCommandExists("python")
    ValidateCommandExists("pip")
    ValidateCommandExists("g++")
    ValidateCommandExists("gcc")

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
                projectType=args.Type,
                recreate=args.IsForce,
                buildOptions=args.BuildOptions,
            )

    if args.IsTest:
        if args.Project in CppProjectNames:
            if args.Project == "libraries":
                # Special case for libraries project as it contains multiple tests
                if args.Module in ["all", "core"]:
                    RunLibrariesTest(
                        projectDir="core",
                        projectType=args.Type,
                    )

                if args.Module in ["all", "modules"]:
                    RunLibrariesTest(
                        projectDir="modules",
                        projectType=args.Type,
                    )

                if args.Module in ["all", "applications"]:
                    RunLibrariesTest(
                        projectDir="applications",
                        projectType=args.Type,
                    )
        else:
            RunPythonProjectTest(
                projectDir=args.Project,
                filter=args.TestFilter,
            )

    if args.IsRun:
        if args.Project in CppProjectNames:
            RunCppProject(projectDir=args.Project, projectType=args.Type)
        else:
            RunPythonProject(
                projectDir=args.Project,
                force=args.IsForce,
                reset=args.IsReset,
            )

    if args.IsDesigner:
        OpenPyQtDesigner()


if __name__ == "__main__":
    main()
