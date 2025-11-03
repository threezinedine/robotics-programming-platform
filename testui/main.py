import os
import json
import argparse
import subprocess
from typing import Literal
from rich.table import Table
from rich.console import Console

PROJECT_BASE_DIR = os.path.dirname(os.getcwd())
RESULTS_FILE_PATH = os.path.join(
    PROJECT_BASE_DIR,
    "e2e",
    "results.json",
)

if os.name == "nt":
    raise Exception("This script is not supported on Windows.")
else:
    EDITOR_TEST_DIR = os.path.join(
        PROJECT_BASE_DIR,
        "editor",
        "build",
        "dev",
    )
    EDITOR_TEST_EXE = os.path.join(EDITOR_TEST_DIR, "RppEditor_test")


class Test:
    fileName: str = ""
    testCases: list[str] = []

    def __repr__(self) -> str:
        return f"<Test name={self.fileName} testCases={self.testCases} />"


editorTests: list[Test] = []
gruntimeTests: list[Test] = []


class Args:
    def __init__(self) -> None:
        parser = argparse.ArgumentParser(description="Robotics Programming Platform")

        parser.add_argument(
            "--project",
            "-p",
            type=str,
            choices=["editor", "gruntime"],
            default="editor",
            help="Project to run",
        )

        parser.add_argument("--scenario", "-s", type=str)

        self.args = parser.parse_args()

    @property
    def Project(self) -> Literal["editor", "gruntime"]:
        return self.args.project

    @property
    def Scenario(self) -> str | None:
        return self.args.scenario


def ExtractTestsFromFile(filePath: str) -> Test:
    test = Test()
    test.fileName = os.path.basename(filePath)[:-3]  # Remove ".py" extension

    if not os.path.isfile(filePath):
        return test

    with open(filePath, "r") as file:
        lines = file.readlines()
        for line in lines:
            line = line.strip()
            if line.startswith("def test_"):
                test.testCases.append(line[4:-3])  # Remove "def " and "():"

    return test


def RetrieveAllTests() -> None:
    for root, _, files in os.walk(os.path.join(PROJECT_BASE_DIR, "e2e")):
        for file in files:
            if file.startswith("editor_") and file.endswith("_test.py"):
                editorTests.append(ExtractTestsFromFile(os.path.join(root, file)))
            elif file.startswith("gruntime_") and file.endswith("_test.py"):
                gruntimeTests.append(ExtractTestsFromFile(os.path.join(root, file)))


def RunCommand(command: str, cwd: str | None = None) -> None:
    subprocess.run(command, shell=True, cwd=cwd)


def main():
    args = Args()

    RetrieveAllTests()

    if os.path.exists(RESULTS_FILE_PATH):
        os.remove(RESULTS_FILE_PATH)

        with open(RESULTS_FILE_PATH, "w") as resultsFile:
            resultsFile.write("[]")

    if args.Project == "editor":
        for test in editorTests:
            for testCase in test.testCases:
                if args.Scenario is None or args.Scenario == testCase:
                    RunCommand(
                        f"{EDITOR_TEST_EXE} {test.fileName} {testCase}",
                        cwd=EDITOR_TEST_DIR,
                    )

    table = Table(
        title="🚀 [bold magenta]TEST RESULTS[/]",
        show_header=True,
        header_style="bold blue",
    )

    table.add_column("Scenario", style="dim", width=40)
    table.add_column("Status", justify="center", width=10)
    table.add_column("Error Message", width=40)

    with open(RESULTS_FILE_PATH, "r") as resultsFile:
        resultsContent = json.loads(resultsFile.read())
        for result in resultsContent:
            assert "name" in result, "Each result must have a 'scenario' field."
            assert "status" in result, "Each result must have a 'status' field."
            assert "error" in result, "Each result must have an 'error' field."

            table.add_row(
                result["name"],
                "[green]PASSED[/]" if result["status"] else "[red]FAILED[/]",
                result["error"],
            )

        Console().print(table)


if __name__ == "__main__":
    try:
        main()
    except Exception as e:
        print(f"Error: {e}")
        exit(1)
