import argparse
from .logger import logger
from logging import DEBUG, INFO


PythonProjectNames = ["autogen"]
CppProjectNames = ["runtime", "gruntime", "libraries", "editor"]
ProjectNames = [*PythonProjectNames, *CppProjectNames]
Types = ["dev", "prod"]


class Args:
    """
    Extract all the input command line configuration for the application. This
    is just a wrapper around standard argparse functionality of the Python
    standard library.
    """

    def __init__(self) -> None:
        parser = argparse.ArgumentParser(
            description="Utility for working with configuration arguments"
        )

        # ================ Common Arguments ================
        parser.add_argument(
            "-p",
            "--project",
            type=str,
            choices=ProjectNames,
            default="runtime",
            help="Specify the project name (default: runtime)",
        )

        parser.add_argument(
            "-t",
            "--type",
            type=str,
            choices=Types,
            default="dev",
            help="Specify the build type (default: dev)",
        )

        parser.add_argument(
            "-f",
            "--force",
            action="store_true",
            help="Force the operation, ignoring any warnings",
        )

        parser.add_argument(
            "-v",
            "--verbose",
            action="store_true",
            help="Enable verbose output for debugging purposes",
        )

        # ================ Action Sub-Commands ================
        subparsers = parser.add_subparsers(
            dest="command",
            help="Action to perform",
            required=True,
        )

        installPackagesParser = subparsers.add_parser(
            "install",
            help="Install the required packages for the specified project",
        )

        installPackagesParser.add_argument(
            "packages",
            type=str,
            nargs="+",  # at least one package is required
            help="The name of the packages to install",
        )

        buildParser = subparsers.add_parser(
            "build",
            help="Build the specified project",
        )

        buildParser.add_argument(
            "-o",
            "--options",
            type=str,
            nargs="+",
            help="Additional options for the build process",
        )

        runSubParser = subparsers.add_parser(
            "run",
            help="Run the specified project",
        )

        subparsers.add_parser(
            "designer",
            help="Launch the GUI designer tool",
        )

        runSubParser.add_argument(
            "-r",
            "--reset",
            action="store_true",
            help="Reset the project state before running",
        )

        testParser = subparsers.add_parser(
            "test",
            help="Run the tests for the specified project",
        )

        testParser.add_argument(
            "--filter",
            type=str,
            default=None,
            help="Optional filter to run specific tests only (default: None)",
        )

        testParser.add_argument(
            "-m",
            "--module",
            type=str,
            default=None,
            help="Optional module to build (default: None) only works for `libraries`",
        )

        self.args = parser.parse_args()

        if self.args.verbose:
            logger.setLevel(DEBUG)
        else:
            logger.setLevel(INFO)

    @property
    def Project(self) -> str:
        """
        Returns the project name specified in the command line arguments.
        """
        return self.args.project

    @property
    def Type(self) -> str:
        """
        Returns the build type which is either 'dev' or 'prod'.
        """
        return self.args.type

    @property
    def IsForce(self) -> bool:
        """
        If  the force flag is set, this will be true then all operations
        should be forced to run (igonring the cache).
        """
        return self.args.force

    @property
    def IsPackageInstall(self) -> bool:
        """
        Returns true if the command is to install packages.
        """
        return self.args.command == "install"

    @property
    def InstalledPackages(self) -> list[str]:
        """
        Returns the list of packages to install.
        """
        if self.IsPackageInstall:
            return self.args.packages
        return []

    @property
    def IsBuild(self) -> bool:
        """
        Returns true if the command is to build the project.
        """
        return self.args.command == "build"

    @property
    def Toolchain(self) -> str:
        """
        Returns the toolchain to use for building the project.
        """
        if self.IsBuild:
            return self.args.toolchain
        return "msvc"

    @property
    def IsRun(self) -> bool:
        """
        Returns true if the command is to run the project.
        """
        return self.args.command == "run"

    @property
    def IsTest(self) -> bool:
        """
        Returns true if the command is to run the tests.
        """
        return self.args.command == "test"

    @property
    def IsReset(self) -> bool:
        """
        Returns true if the reset flag is set when running the project.
        """
        if self.IsRun:
            return self.args.reset
        return False

    @property
    def IsDesigner(self) -> bool:
        """
        Returns true if the command is to launch the GUI designer tool.
        """
        return self.args.command == "designer"

    @property
    def TestFilter(self) -> str | None:
        """
        Returns the optional filter to run specific tests only.
        """
        if self.IsTest:
            return self.args.filter
        return None

    @property
    def BuildOptions(self) -> list[str]:
        """
        Returns additional options for the build process.
        """
        if self.IsBuild and self.args.options:
            return self.args.options
        return []

    @property
    def Module(self) -> str:
        """
        Returns the optional module to build.
        """
        if self.IsTest and self.args.module:
            return self.args.module
        return "all"  # Default to 'all' if not specified
