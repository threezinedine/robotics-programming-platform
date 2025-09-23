from abc import ABC, abstractmethod
from parser.py_enum import PyEnum, PyEnumConstant
from parser.py_function import PyFunction
from parser.py_method import PyMethod
from parser.py_parameter import PyParameter
from parser.py_struct import PyStruct
from parser.py_field import PyField
from parser import CStruct
from parser.common import AccessType


class IAssert(ABC):
    """
    The main interface for checking the parsed structures from the C/C++ source code.
    """

    def __init__(
        self,
        name: str,
        comment: str | None = None,
        annotations: list[str] | None = None,
    ) -> None:
        self.name = name
        self.comment = comment
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

        if self.comment is not None:
            assert hasattr(obj, "comment") and isinstance(
                getattr(obj, "comment", None), str
            ), "The provided structure does not have a 'comment' attribute of type str."

            assert (
                getattr(obj, "comment", "") == self.comment
            ), f"Expected comment '{self.comment}', but got '{getattr(obj, 'comment', '')}'."

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
        comment: str | None = None,
        annotations: list[str] | None = None,
    ) -> None:
        super().__init__(name, comment, annotations)
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
        comment: str | None = None,
        annotations: list[str] | None = None,
    ) -> None:
        super().__init__(name, comment, annotations)
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
        comment: str | None = None,
        annotations: list[str] | None = None,
    ) -> None:
        super().__init__(name, comment, annotations)
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


class ParameterAssert(IAssert):
    """
    A concrete implementation of the IAssert interface for checking method parameters.
    """

    def __init__(
        self,
        name: str,
        type: str,
        hasDefaultValue: bool = False,
        comment: str | None = None,
        annotations: list[str] | None = None,
    ) -> None:
        super().__init__(name, comment, annotations)
        self.type = type
        self.hasDefaultValue = hasDefaultValue

    def _AssertImpl(self, obj: CStruct) -> None:
        assert isinstance(
            obj, PyParameter
        ), "The provided structure is not a parameter."

        parameter: PyParameter = obj  # type: ignore

        assert (
            parameter.type == self.type
        ), f"Expected parameter type '{self.type}', but got '{parameter.type}'."

        assert (
            parameter.hasDefaultValue == self.hasDefaultValue
        ), f"Expected parameter hasDefaultValue '{self.hasDefaultValue}', but got '{parameter.hasDefaultValue}'."


class FunctionAssert(IAssert):
    """
    A concrete implementation of the IAssert interface for checking free functions.
    """

    def __init__(
        self,
        name: str,
        returnType: str,
        parameters: list[ParameterAssert] | None = None,
        comment: str | None = None,
        returnComment: str | None = None,
        annotations: list[str] | None = None,
    ) -> None:
        super().__init__(name, comment, annotations)
        self.returnType = returnType
        self.returnComment = returnComment
        self.parameters = parameters if parameters is not None else []

    def _AssertImpl(self, obj: CStruct) -> None:
        assert isinstance(obj, PyFunction), "The provided structure is not a function."

        function: PyFunction = obj  # type: ignore

        assert (
            function.returnType == self.returnType
        ), f"Expected function return type '{self.returnType}', but got '{function.returnType}'."

        assert len(function.parameters) == len(
            self.parameters
        ), f"Expected {len(self.parameters)} parameters, but got {len(function.parameters)}."
        for assertion, parameter in zip(self.parameters, function.parameters):
            assertion.Assert(parameter)

        if self.returnComment is not None:
            assert (
                function.returnComment == self.returnComment
            ), f"Expected return comment '{self.returnComment}', but got '{function.returnComment}'."


class MethodAssert(IAssert):
    """
    A concrete implementation of the IAssert interface for checking methods.
    """

    def __init__(
        self,
        name: str,
        returnType: str,
        access: AccessType | None = None,
        parameters: list[ParameterAssert] | None = None,
        comment: str | None = None,
        returnComment: str | None = None,
        annotations: list[str] | None = None,
    ) -> None:
        super().__init__(name, comment, annotations)
        self.returnType = returnType
        self.returnComment = returnComment
        self.access = access if access is not None else "public"
        self.parameters = parameters if parameters is not None else []

    def _AssertImpl(self, obj: CStruct) -> None:
        assert isinstance(obj, PyMethod), "The provided structure is not a method."

        method: PyMethod = obj  # type: ignore

        assert (
            method.returnType == self.returnType
        ), f"Expected method return type '{self.returnType}', but got '{method.returnType}'."

        assert (
            method.access == self.access
        ), f"Expected method access '{self.access}', but got '{method.access}'."

        assert len(method.parameters) == len(
            self.parameters
        ), f"Expected {len(self.parameters)} parameters, but got {len(method.parameters)}."
        for assertion, parameter in zip(self.parameters, method.parameters):
            assertion.Assert(parameter)

        if self.returnComment is not None:
            assert (
                method.returnComment == self.returnComment
            ), f"Expected return comment '{self.returnComment}', but got '{method.returnComment}'."


class StructAssert(IAssert):
    """
    A concrete implementation of the IAssert interface for checking structures.
    """

    def __init__(
        self,
        name: str,
        fields: list[FieldAssert | MethodAssert] = [],
        comment: str | None = None,
        annotations: list[str] | None = None,
    ) -> None:
        super().__init__(name, comment, annotations)
        self.fields = fields

    def _AssertImpl(self, obj: CStruct) -> None:
        assert isinstance(obj, PyStruct), "The provided structure is not a struct."

        struct: PyStruct = obj  # type: ignore

        assert len(struct.fields) == len(
            self.fields
        ), f"Expected {len(self.fields)} fields, but got {len(struct.fields)}."

        for assertion, field in zip(self.fields, struct.fields):
            assertion.Assert(field)
