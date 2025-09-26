from typing import Generator
import pytest  # type: ignore
from utils import (
    DependencyInjection,
    Depend,
    AsSingleton,
    GetObject,
    AsTransient,
)


class SingletonObject:
    """
    Used only for singleton and transient object tests.

    Parameters
    ----------
    value : int
        The value to set the count to, used for checking passed parameters.
    """

    numberOfCount: int = 0

    def __init__(self, value: int = 0) -> None:
        SingletonObject.numberOfCount += 1
        self.count = SingletonObject.numberOfCount
        self.value = value


@Depend(SingletonObject)
class DependentSingleton:
    """
    A singleton object which depends on another singleton object.
    """

    numberOfCount: int = 0

    def __init__(self, singleton: SingletonObject) -> None:
        DependentSingleton.numberOfCount += 1
        self.singleton = singleton


class TransientObject:
    """
    A transient object which is not a singleton.
    """

    numberOfCount: int = 0

    def __init__(self, value: int = 0) -> None:
        self.value = value
        TransientObject.numberOfCount += 1


@pytest.fixture
def setup() -> Generator[None, None, None]:
    # Reset the singleton registry before each test
    DependencyInjection._singletons = {}  # type: ignore
    SingletonObject.numberOfCount = 0
    DependentSingleton.numberOfCount = 0
    yield
    # Cleanup after test
    DependencyInjection._singletons = {}  # type: ignore
    SingletonObject.numberOfCount = 0
    DependentSingleton.numberOfCount = 0


def test_register_singleton(setup: None) -> None:
    assert (
        SingletonObject.numberOfCount == 0
    ), "Count should be 0 before any objects are made."

    AsSingleton(SingletonObject, value=5)

    assert (
        SingletonObject.numberOfCount == 0
    ), "Count should still be 0 since no singleton instance should have been created yet."

    obj1 = GetObject(SingletonObject)

    assert obj1 is not None, "Singleton object should not be None."
    assert (
        SingletonObject.numberOfCount == 1
    ), "Count should be 1 since one singleton instance should have been created."
    assert (
        obj1.value == 5
    ), "Singleton object should have been initialized with value 5."

    obj2 = GetObject(SingletonObject)
    assert obj1 is obj2, "Both singleton objects should be the same instance."
    assert (
        SingletonObject.numberOfCount == 1
    ), "Count should be 1 since only one singleton instance should have been created."
    assert (
        obj2.value == 5
    ), "Singleton object should have been initialized with value 5."


def test_register_existed_singleton(setup: None) -> None:
    assert (
        SingletonObject.numberOfCount == 0
    ), "Count should be 0 before registering the singleton again."

    AsSingleton(int, SingletonObject, 20)
    AsSingleton(SingletonObject, value=10)

    obj = GetObject(SingletonObject)
    assert obj is not None, "Singleton object should not be None."
    assert (
        SingletonObject.numberOfCount == 1
    ), "Count should still be 1 since only one singleton instance should have been created."

    assert (
        obj.value == 10
    ), "Singleton object should still have the initial value of 10."


def test_add_dependency_between_singletons(setup: None) -> None:
    AsSingleton(DependentSingleton)
    AsSingleton(SingletonObject, value=10)

    dependent = GetObject(DependentSingleton)

    assert dependent is not None, "Dependent singleton should not be None."
    assert (
        DependentSingleton.numberOfCount == 1
    ), "Count should be 1 since one dependent singleton instance should have been created."

    assert (
        dependent.singleton is not None
    ), "The singleton dependency should have been injected and should not be None."

    assert (
        SingletonObject.numberOfCount == 1
    ), "Count should be 1 since only one singleton instance should have been created."


def test_get_non_existent_singleton(setup: None) -> None:
    with pytest.raises(ValueError):
        DependencyInjection.GetObject("NonExistentSingleton")


def test_register_singleton_using_object(setup: None) -> None:
    obj = SingletonObject(value=30)
    AsSingleton(obj, SingletonObject)

    assert (
        SingletonObject.numberOfCount == 1
    ), "Count should be 1 since one singleton instance should have been created."

    retrieved_obj = GetObject(SingletonObject)
    assert (
        retrieved_obj is obj
    ), "The retrieved singleton should be the same as the registered object."
    assert (
        retrieved_obj.value == 30
    ), "The retrieved singleton should have the value of 30."


def test_register_via_wrapper(setup: None) -> None:
    AsSingleton(SingletonObject, value=42)
    AsSingleton(DependentSingleton)

    assert (
        SingletonObject.numberOfCount == 0
    ), "Count should be 0 since no singleton instance should have been created yet."

    dependent = GetObject(DependentSingleton)
    assert dependent is not None, "Dependent singleton should not be None."
    assert (
        DependentSingleton.numberOfCount == 1
    ), "Count should be 1 since one dependent singleton instance should have been created."
    assert (
        SingletonObject.numberOfCount == 1
    ), "Count should be 1 since one singleton instance should have been created."


def test_wrapper_with_object(setup: None) -> None:
    obj = SingletonObject(value=55)
    AsSingleton(obj, interfaceType=SingletonObject)

    GetObject(SingletonObject)

    assert (
        SingletonObject.numberOfCount == 1
    ), "Count should be 1 since one singleton instance should have been created."


def test_wrapper_with_object_no_interface(setup: None) -> None:
    obj = SingletonObject(value=55)
    with pytest.raises(ValueError):
        AsSingleton(obj)  # No interfaceType provided and obj is not a class type


def test_wrapper_with_transient(setup: None) -> None:
    AsTransient(TransientObject)

    obj1 = GetObject(TransientObject, value=1)

    assert obj1 is not None, "Transient object should not be None."
    assert (
        obj1.value == 1
    ), "Transient object should have been initialized with value 1."

    obj2 = GetObject(TransientObject)
    assert obj2 is not None, "Transient object should not be None."
    assert obj2 is not obj1, "Both transient objects should be different instances."
    assert (
        obj2.value == 0
    ), "Transient object should have been initialized with value 0."

    assert (
        TransientObject.numberOfCount == 2
    ), "Count should be 2 since two transient instances should have been created."


def test_register_transient_with_same_interface_as_singleton(setup: None) -> None:
    AsSingleton(SingletonObject, value=10)
    with pytest.raises(ValueError):
        AsTransient(TransientObject, SingletonObject, value=20)


def test_register_overwrite_transient(setup: None) -> None:
    AsTransient(TransientObject, value=1)
    AsTransient(TransientObject, value=2)


def test_register_singleton_with_existed_transient(setup: None) -> None:
    AsTransient(TransientObject, value=1)
    with pytest.raises(ValueError):
        AsSingleton(SingletonObject, TransientObject, value=10)


def test_get_singleton_with_args(setup: None) -> None:
    AsSingleton(SingletonObject, value=50)

    obj = GetObject(SingletonObject, value=99)

    assert obj is not None, "Singleton object should not be None."
    assert (
        SingletonObject.numberOfCount == 1
    ), "Count should be 1 since one singleton instance should have been created."
    assert (
        obj.value == 50
    ), "Singleton object should have been initialized with value 50."
