from abc import ABC, abstractmethod
from typing import Any


class Assert(ABC):
    """
    Base class for the ui assertions (file system related which differ from UIAssert).
    Inherit from this class and implement the _Assert method.
    1. Create a class that inherits from Assert.
    2. Implement the _Assert method with your custom assertions.
    3. Instantiate your class and call the Assert method to perform the assertions.
    Example:
        class ProjectDescriptionAssert(Assert):
            def _Assert(self) -> None:
                assert os.path.exists("init.rpproj") and os.path.isfile("init.rpproj")

    """

    def __init__(self) -> None:
        super().__init__()

    def Assert(self, obj: Any = None) -> None:
        """
        The test will call this method to perform the assertions.
        """
        self._Assert(obj)

    @abstractmethod
    def _Assert(self, obj: Any = None) -> None:
        """
        Implement your custom assertions here. This method will be called by the Assert method.

        Parameters
        ----------
        object : Any
            An optional object that can be used in the assertions.
        """
        raise NotImplementedError()
