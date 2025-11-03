import os
import argparse
import subprocess
from typing import Literal

PROJECT_BASE_DIR = os.path.dirname(os.getcwd())

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

        self.args = parser.parse_args()

    @property
    def Project(self) -> Literal["editor", "gruntime"]:
        return self.args.project


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

    if args.Project == "editor":
        RunCommand(
            f"{EDITOR_TEST_EXE} {editorTests[0].fileName} {editorTests[0].testCases[0]}",
            cwd=EDITOR_TEST_DIR,
        )


if __name__ == "__main__":
    try:
        main()
    except Exception as e:
        print(f"Error: {e}")
        exit(1)
