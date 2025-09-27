import os
from typing import Any
from Engine import ToString_ProjectDescription, ProjectDescription
from constants import PROJECT_FILE_NAME
from Engine import ToString_FunctionDescription, FunctionDescription


class ProjectBuilder:
    """
    Assume that the project directory in test is alwayas cwd.
    """

    def __init__(self, projectName: str) -> None:
        self.projectName = projectName
        self.projectDir = os.path.join(os.getcwd(), projectName)
        self.projectFile = os.path.join(self.projectDir, PROJECT_FILE_NAME)

        self.functionBuilders: list["FunctionBuilder"] = []

    def AddFunction(self, builder: "FunctionBuilder") -> "ProjectBuilder":
        """
        Add a function to the project.
        """
        self.functionBuilders.append(builder)
        return self

    def Build(self) -> None:
        """
        Only actually run build of all needed environment here. Other defined via methods
        will be delegated to the methods.
        """
        if not os.path.exists(self.projectDir):
            os.makedirs(self.projectDir)

        projectDesc = ProjectDescription()
        projectDesc.name = self.projectName

        with open(self.projectFile, "w") as f:
            f.write(ToString_ProjectDescription(projectDesc))

        for functionBuilder in self.functionBuilders:
            functionBuilder.Build(self)


class FunctionBuilder:
    def __init__(self, name: str) -> None:
        self.name = name

    def Build(self, obj: Any) -> None:
        assert isinstance(obj, ProjectBuilder), "The object must be a ProjectBuilder."

        functionDescript = FunctionDescription()
        functionDescript.name = self.name

        with open(
            os.path.join(
                obj.projectDir,
                f"{self.name}.rppfunc",
            ),
            "w",
        ) as f:
            f.write(ToString_FunctionDescription(functionDescript))
