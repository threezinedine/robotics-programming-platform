from abc import ABC, abstractmethod
from parser.py_enum import PyEnum, PyEnumConstant
from parser import CStruct


class IAssert(ABC):
    """
    The main interface for checking the parsed structures from the C/C++ source code.
    """

    def __init__(self, annotations: list[str] | None = None) -> None:
        self.annotations = annotations if annotations is not None else []

    @abstractmethod
    def _AssertImpl(self, enum: CStruct) -> None:
        """
        Abstract method to be implemented by subclasses for asserting specific conditions on the parsed structures.

        Parameters
        ----------
            enum (CStruct)
                The parsed structure to be checked.
        """
        pass

    def Assert(self, enum: CStruct) -> None:
        """
        Calls the concrete implementation of the assertion.

        Parameters
        ----------
            enum (CStruct)
                The parsed structure to be checked.
        """
        self._AssertImpl(enum)

        # Additional common assertions can be added here.
        if len(self.annotations) > 0:
            assert hasattr(enum, "annotations") and isinstance(
                getattr(enum, "annotations", list), list
            ), "The provided structure does not have an 'annotations' attribute of type list."

            assert set(getattr(enum, "annotations", [])) == set(
                self.annotations
            ), f"Expected annotations '{self.annotations}', but got '{getattr(enum, 'annotations', [])}'."


class EnumConstantsAssert(IAssert):
    def __init__(
        self,
        name: str,
        value: int,
        annotations: list[str] | None = None,
    ) -> None:
        super().__init__(annotations)
        self.name = name
        self.value = value

    def _AssertImpl(self, enum: CStruct) -> None:
        assert isinstance(
            enum, PyEnumConstant
        ), "The provided structure is not an enumeration constant."

        assert (
            enum.name == self.name
        ), f"Expected enum constant name '{self.name}', but got '{enum.name}'."

        assert (
            enum.value == self.value
        ), f"Expected enum constant value '{self.value}', but got '{enum.value}'."


class EnumAssert(IAssert):
    """
    A concrete implementation of the IAssert interface for checking enumerations.
    """

    def __init__(
        self,
        name: str,
        constants: list[EnumConstantsAssert],
        annotations: list[str] | None = None,
    ) -> None:
        super().__init__(annotations)
        self.name = name
        self.constants = constants

    def _AssertImpl(self, enum: CStruct) -> None:
        assert isinstance(enum, PyEnum), "The provided structure is not an enumeration."

        assert (
            enum.name == self.name
        ), f"Expected enum name '{self.name}', but got '{enum.name}'."

        assert len(enum.constants) == len(
            self.constants
        ), f"Expected {len(self.constants)} constants, but got {len(enum.constants)}."

        for assertion, constant in zip(self.constants, enum.constants):
            assertion.Assert(constant)
