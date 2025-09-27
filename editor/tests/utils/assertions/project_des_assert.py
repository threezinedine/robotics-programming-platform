import os
from typing import Any
from .assertion import Assert
from Engine import FromString_ProjectDescription
from constants import PROJECT_FILE_NAME


class ProjectDescriptionAssert(Assert):
    def __init__(self, projectDir: str, projectName: str) -> None:
        super().__init__()
        self.projectDir = projectDir
        self.projectName = projectName

    def _Assert(self, obj: Any = None) -> None:
        finalProjectDir = os.path.join(self.projectDir, self.projectName)
        projectFile = os.path.join(finalProjectDir, PROJECT_FILE_NAME)
        assert os.path.exists(
            finalProjectDir
        ), f"Project directory does not exist: {finalProjectDir}"
        assert os.path.exists(
            projectFile
        ), f"Project file does not exist: {projectFile}"
        assert os.path.isfile(projectFile), f"Project file is not a file: {projectFile}"

        with open(projectFile, "r") as f:
            projectDesc = FromString_ProjectDescription(f.read())
            assert projectDesc.name == self.projectName
