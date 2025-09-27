from typing import Any
from .assertion import Assert
from .project_des_assert import ProjectDescriptionAssert
from .function_file_assert import FunctionFileAssert


class ProjectAssert(Assert):
    def __init__(self, projectDesc: ProjectDescriptionAssert) -> None:
        super().__init__()
        self.projectDescription = projectDesc
        self.functionFiles: list[FunctionFileAssert] = []

    def AddFunctionFile(self, function: FunctionFileAssert) -> "ProjectAssert":
        self.functionFiles.append(function)
        return self

    def _Assert(self, obj: Any = None) -> None:
        self.projectDescription.Assert(obj)

        for function in self.functionFiles:
            function.Assert(self.projectDescription.projectName)
