import os
from typing import Any
from .assertion import Assert


class FunctionFileAssert(Assert):
    def __init__(self, functionName: str) -> None:
        super().__init__()
        self.functionName = functionName

    def _Assert(self, obj: Any = None) -> None:
        assert isinstance(
            obj,
            str,
        ), "FunctionFileAssert requires the project directory as a string argument."

        functionFile = os.path.join(
            os.getcwd(),
            str(obj),
            self.functionName + ".rppfunc",
        )
        assert os.path.exists(
            functionFile
        ), f"Function file does not exist: {functionFile}"
        assert os.path.isfile(
            functionFile
        ), f"Function file is not a file: {functionFile}"
