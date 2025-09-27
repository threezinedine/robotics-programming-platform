import os
from Engine import (
    Function,
    FunctionDescription,
    ToString_FunctionDescription,
    FromString_FunctionDescription,
)
from utils.dependency_injection import Depend
from .project_state import ProjectStateModel


@Depend(ProjectStateModel)
class FunctionModel:
    """
    The middle layer between the function data and the view model which deals with the function data.
    """

    def __init__(self, projectStateModel: ProjectStateModel) -> None:
        self.projectStateModel = projectStateModel
        self.description: FunctionDescription | None = None
        self.function: Function | None = None

    def CreateNewFunction(self, functionName: str) -> None:
        self.description = FunctionDescription()
        self.description.name = functionName
        self.function = Function.CreateFunction(self.description)

    def FromFile(self, filePath: str) -> None:
        with open(filePath, "r") as f:
            content = f.read()
            self.description = FromString_FunctionDescription(content)
            self.function = Function.CreateFunction(self.description)

    def Save(self) -> None:
        assert self.function is not None, "Function is not created yet."
        assert self.description is not None, "Function description is not created yet."
        assert (
            self.projectStateModel.CurrentProject is not None
        ), "No project is currently loaded."

        filePath = os.path.join(
            self.projectStateModel.projectDir,
            f"{self.description.name}.rppfunc",
        )

        with open(filePath, "w") as f:
            f.write(ToString_FunctionDescription(self.description))

    def ChangeName(self, newName: str) -> None:
        assert self.description is not None, "Function description is not created yet."
        assert (
            self.projectStateModel.CurrentProject is not None
        ), "No project is currently loaded."

        oldFilePath = os.path.join(
            self.projectStateModel.projectDir,
            f"{self.description.name}.rppfunc",
        )
        newFilePath = os.path.join(
            self.projectStateModel.projectDir,
            f"{newName}.rppfunc",
        )

        os.rename(oldFilePath, newFilePath)
        self.description.name = newName
