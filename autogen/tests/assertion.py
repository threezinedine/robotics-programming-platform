from abc import ABC, abstractmethod
from parser.py_enum import PyEnum, PyEnumConstant
from parser.py_method import PyMethod
from parser.py_struct import PyStruct
from parser.py_field import PyField
from parser import CStruct
from parser.common import AccessType


class IAssert(ABC):
    """
    The main interface for checking the parsed structures from the C/C++ source code.
    """

    def __init__(self, name: str, annotations: list[str] | None = None) -> None:
        self.name = name
        self.annotations = annotations if annotations is not None else []

    @abstractmethod
    def _AssertImpl(self, obj: CStruct) -> None:
        """
        Abstract method to be implemented by subclasses for asserting specific conditions on the parsed structures.

        Parameters
        ----------
            obj (CStruct)
                The parsed structure to be checked.
        """
        pass

    def Assert(self, obj: CStruct) -> None:
        """
        Calls the concrete implementation of the assertion.

        Parameters
        ----------
            enum (CStruct)
                The parsed structure to be checked.
        """
        assert hasattr(obj, "name") and isinstance(
            getattr(obj, "name", None), str
        ), "The provided structure does not have a 'name' attribute of type str."

        assert (
            getattr(obj, "name", "") == self.name
        ), f"Expected name '{self.name}', but got '{getattr(obj, 'name', '')}'."

        self._AssertImpl(obj)

        # Additional common assertions can be added here.
        if len(self.annotations) > 0:
            assert hasattr(obj, "annotations") and isinstance(
                getattr(obj, "annotations", list), list
            ), "The provided structure does not have an 'annotations' attribute of type list."

            assert set(getattr(obj, "annotations", [])) == set(
                self.annotations
            ), f"Expected annotations '{self.annotations}', but got '{getattr(obj, 'annotations', [])}'."


class EnumConstantsAssert(IAssert):
    def __init__(
        self,
        name: str,
        value: int,
        annotations: list[str] | None = None,
    ) -> None:
        super().__init__(name, annotations)
        self.value = value

    def _AssertImpl(self, obj: CStruct) -> None:
        assert isinstance(
            obj, PyEnumConstant
        ), "The provided structure is not an enumeration constant."

        enumConstant: PyEnumConstant = obj  # type: ignore

        assert (
            enumConstant.value == self.value
        ), f"Expected enum constant value '{self.value}', but got '{enumConstant.value}'."


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
        super().__init__(name, annotations)
        self.constants = constants

    def _AssertImpl(self, obj: CStruct) -> None:
        assert isinstance(obj, PyEnum), "The provided structure is not an enumeration."

        enum: PyEnum = obj  # type: ignore

        assert len(enum.constants) == len(
            self.constants
        ), f"Expected {len(self.constants)} constants, but got {len(enum.constants)}."

        for assertion, constant in zip(self.constants, enum.constants):
            assertion.Assert(constant)


class FieldAssert(IAssert):
    """
    A concrete implementation of the IAssert interface for checking fields.
    """

    def __init__(
        self,
        name: str,
        type: str,
        access: AccessType | None = None,
        annotations: list[str] | None = None,
    ) -> None:
        super().__init__(name, annotations)
        self.type = type
        self.access = access if access is not None else "public"

    def _AssertImpl(self, obj: CStruct) -> None:
        assert isinstance(obj, PyField), "The provided structure is not a field."

        field: PyField = obj  # type: ignore

        assert (
            field.type == self.type
        ), f"Expected field type '{self.type}', but got '{field.type}'."

        assert (
            field.access == self.access
        ), f"Expected field access '{self.access}', but got '{field.access}'."


class MethodAssert(IAssert):
    """
    A concrete implementation of the IAssert interface for checking methods.
    """

    def __init__(
        self,
        name: str,
        returnType: str,
        access: AccessType | None = None,
        annotations: list[str] | None = None,
    ) -> None:
        super().__init__(name, annotations)
        self.returnType = returnType
        self.access = access if access is not None else "public"

    def _AssertImpl(self, obj: CStruct) -> None:
        assert isinstance(obj, PyMethod), "The provided structure is not a method."

        method: PyMethod = obj  # type: ignore

        assert (
            method.return_type == self.returnType
        ), f"Expected method return type '{self.returnType}', but got '{method.return_type}'."

        assert (
            method.access == self.access
        ), f"Expected method access '{self.access}', but got '{method.access}'."


class StructAssert(IAssert):
    """
    A concrete implementation of the IAssert interface for checking structures.
    """

    def __init__(
        self,
        name: str,
        fields: list[FieldAssert | MethodAssert] = [],
        annotations: list[str] | None = None,
    ) -> None:
        super().__init__(name, annotations)
        self.fields = fields

    def _AssertImpl(self, obj: CStruct) -> None:
        assert isinstance(obj, PyStruct), "The provided structure is not a struct."

        struct: PyStruct = obj  # type: ignore

        assert len(struct.fields) == len(
            self.fields
        ), f"Expected {len(self.fields)} fields, but got {len(struct.fields)}."

        for assertion, field in zip(self.fields, struct.fields):
            assertion.Assert(field)
