from config.dependency import InstallCppDependencies
from config.utils.validation_utils import ValidateCommandExists, ValidateEnvDirExists
from config.args import Args, CppProjectNames, PythonProjectNames
from config.utils.python_project_utils import InstallPackages, RunPythonProject
from config.utils.cpp_project_utils import BuildProject, RunCppProject


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
            )

    if args.IsRun:
        if args.Project in CppProjectNames:
            ValidateCommandExists("msbuild")
            RunCppProject(projectDir=args.Project, projectType=args.Type)
        else:
            RunPythonProject(projectDir=args.Project)


if __name__ == "__main__":
    main()
