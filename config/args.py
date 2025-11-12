import argparse
from typing import Any, Dict
from .logger import logger
from logging import DEBUG, INFO


PythonProjectNames = ["autogen", "testui", "templategen"]
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

        testParser.add_argument(
            "--scenario",
            "-s",
            type=str,
            help="Specify the scenario to use when running the tests (default: empty_scenario)",
        )

        testParser.add_argument(
            "--build",
            action="store_true",
            help="Build the project before running tests (for cpp projects only)",
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
    def IsPackageInstall(self) -> bool:
        """
        Returns true if the command is to install packages.
        """
        return self.args.command == "install"

    @property
    def IsBuild(self) -> bool:
        """
        Returns true if the command is to build the project.
        """
        return self.args.command == "build"

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
    def IsTestBuild(self) -> bool:
        """
        Returns true if the build flag is set when running the tests.
        """
        if self.IsTest:
            return self.args.build
        return False

    def ToDict(self) -> Dict[str, Any]:
        """
        Converts the arguments to a dictionary.
        """
        return vars(self.args)
